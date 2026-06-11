#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_worksheet_verification.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view verification_command = "SHARDROUTEWORKSHEETVERIFYJSON";

} // namespace

int main() {
    const std::string verification =
        minikv::shard_route_preview_worksheet_verification::format_worksheet_verification_json();
    minikv::test_support::assert_contains(verification,
                                          "\"contract\":\"shard-route-preview-worksheet-verification.v1\"");
    minikv::test_support::assert_contains(verification, "\"command\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    minikv::test_support::assert_contains(
        verification,
        "\"verificationMode\":\"read-only-shard-preview-worksheet-boundary-verification\"");
    minikv::test_support::assert_contains(verification,
                                          "\"sourceVerificationCommand\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    minikv::test_support::assert_contains(verification, "\"sourceVerificationReleaseVersion\":\"v510\"");
    minikv::test_support::assert_contains(verification, "\"sourceVerificationPublishedStageCount\":25");
    minikv::test_support::assert_contains(verification, "\"sourceVerificationChainComplete\":true");
    minikv::test_support::assert_contains(verification, "\"verifiedSourceReleaseRangeStart\":\"v486\"");
    minikv::test_support::assert_contains(verification, "\"verifiedSourceReleaseRangeEnd\":\"v510\"");
    minikv::test_support::assert_contains(verification, "\"operatorEntrySlotCount\":25");
    minikv::test_support::assert_contains(verification, "\"stageChainHelperApplied\":true");
    minikv::test_support::assert_contains(verification, "\"verificationCheckCount\":15");
    minikv::test_support::assert_contains(verification, "\"verificationPassedCount\":15");
    minikv::test_support::assert_contains(verification, "\"readyForOperatorEntryWorksheet\":true");
    minikv::test_support::assert_contains(verification, "\"readyForManualEvidenceEntry\":false");
    minikv::test_support::assert_contains(verification, "\"readyForLiveExecution\":false");
    minikv::test_support::assert_contains(verification, "\"readyForProductionExecution\":false");
    minikv::test_support::assert_contains(verification, "\"operatorEntryValuesAccepted\":false");
    minikv::test_support::assert_contains(verification, "\"manualEvidenceImportAllowed\":false");
    minikv::test_support::assert_contains(verification, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(verification, "\"syntheticEvidenceAccepted\":false");
    minikv::test_support::assert_contains(verification, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(verification, "\"routeInstalled\":false");
    minikv::test_support::assert_contains(verification, "\"serviceStarted\":false");
    minikv::test_support::assert_contains(verification, "\"walTouched\":false");
    minikv::test_support::assert_contains(verification, "\"writeRoutingEnabled\":false");
    minikv::test_support::assert_contains(verification, "\"declaredWorksheetVerificationOnly\":true");
    minikv::test_support::assert_contains(verification, "\"singleWorksheetVerifierSectionExpected\":true");
    minikv::test_support::assert_read_only_evidence_boundaries(verification);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewWorksheetVerification\":") == 1);
    minikv::test_support::assert_current_shard_readiness_status(shard);
    minikv::test_support::assert_precheck_upstream_receipt_source_status(shard);
    minikv::test_support::assert_contains(shard, "\"sourceVerificationReleaseVersion\":\"v510\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEWORKSHEETVERIFYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEWORKSHEETVERIFYJSON");

    result = processor.execute("SHARDROUTEWORKSHEETVERIFYJSON");
    assert(result.response == verification);
    assert(store.size() == 0);

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEWORKSHEETVERIFYJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEWORKSHEETVERIFYJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, verification_command);

    return 0;
}
