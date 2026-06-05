#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_import_preflight.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view import_preflight_command = "SHARDROUTEIMPORTPREFLIGHTJSON";

} // namespace

int main() {
    const std::string preflight =
        minikv::shard_route_preview_operator_import_preflight::format_import_preflight_json();
    minikv::test_support::assert_contains(preflight,
                                          "\"contract\":\"shard-route-preview-operator-import-preflight.v1\"");
    minikv::test_support::assert_contains(preflight, "\"command\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    minikv::test_support::assert_contains(
        preflight,
        "\"preflightMode\":\"controlled-read-only-operator-evidence-import-preflight\"");
    minikv::test_support::assert_contains(preflight,
                                          "\"sourceWorksheetCommand\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetReleaseVersion\":\"v535\"");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetFixturePath\":\"fixtures/release/shard-readiness-v535.json\"");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetPublishedStageCount\":25");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetChainComplete\":true");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetReleaseRangeStart\":\"v511\"");
    minikv::test_support::assert_contains(preflight, "\"sourceWorksheetReleaseRangeEnd\":\"v535\"");
    minikv::test_support::assert_contains(preflight, "\"operatorPreflightSlotCount\":25");
    minikv::test_support::assert_contains(preflight, "\"acceptedOperatorEvidenceSlotCount\":0");
    minikv::test_support::assert_contains(preflight, "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(preflight, "\"sharedBoundaryHelperApplied\":true");
    minikv::test_support::assert_contains(preflight, "\"preflightCheckCount\":15");
    minikv::test_support::assert_contains(preflight, "\"preflightPassedCount\":15");
    minikv::test_support::assert_contains(preflight, "\"readyForOperatorEvidenceImportPreflight\":true");
    minikv::test_support::assert_contains(preflight, "\"readyForEvidenceImport\":false");
    minikv::test_support::assert_contains(preflight, "\"readyForManualEvidenceEntry\":false");
    minikv::test_support::assert_contains(preflight, "\"readyForLiveExecution\":false");
    minikv::test_support::assert_contains(preflight, "\"readyForProductionExecution\":false");
    minikv::test_support::assert_contains(preflight, "\"operatorEvidenceValuesAccepted\":false");
    minikv::test_support::assert_contains(preflight, "\"evidenceImportAllowed\":false");
    minikv::test_support::assert_contains(preflight, "\"manualEvidenceImportAllowed\":false");
    minikv::test_support::assert_contains(preflight, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(preflight, "\"syntheticEvidenceAccepted\":false");
    minikv::test_support::assert_contains(preflight, "\"syntheticEvidenceRejected\":true");
    minikv::test_support::assert_contains(preflight, "\"secretValueStored\":false");
    minikv::test_support::assert_contains(preflight, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(preflight, "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(preflight, "\"normalizerExecutes\":false");
    minikv::test_support::assert_contains(preflight, "\"normalizationExecutionAllowed\":false");
    minikv::test_support::assert_contains(preflight, "\"missingValuePolicyBlocksImport\":true");
    minikv::test_support::assert_contains(preflight, "\"importBlockersEnforcedAsMetadata\":true");
    minikv::test_support::assert_contains(preflight, "\"redactionPreserved\":true");
    minikv::test_support::assert_contains(preflight, "\"declaredImportPreflightOnly\":true");
    minikv::test_support::assert_contains(preflight, "\"singleImportPreflightSectionExpected\":true");
    minikv::test_support::assert_read_only_evidence_boundaries(preflight);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewOperatorImportPreflight\":") == 1);
    minikv::test_support::assert_contains(shard, "\"status\":\"route-preview-import-preflight-");
    minikv::test_support::assert_contains(shard, "\"sourceWorksheetReleaseVersion\":\"v535\"");
    minikv::test_support::assert_contains(shard, "\"readyForOperatorEvidenceImportPreflight\":true");
    minikv::test_support::assert_contains(shard, "\"readyForEvidenceImport\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEIMPORTPREFLIGHTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEIMPORTPREFLIGHTJSON");

    result = processor.execute("SHARDROUTEIMPORTPREFLIGHTJSON");
    assert(result.response == preflight);
    assert(store.size() == 0);

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEIMPORTPREFLIGHTJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEIMPORTPREFLIGHTJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, import_preflight_command);

    return 0;
}
