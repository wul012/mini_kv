#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_draft.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view value_draft_command = "SHARDROUTEVALUEDRAFTJSON";

} // namespace

int main() {
    const std::string draft = minikv::shard_route_preview_operator_value_draft::format_value_draft_json();
    minikv::test_support::assert_contains(draft,
                                          "\"contract\":\"shard-route-preview-operator-value-draft.v1\"");
    minikv::test_support::assert_contains(draft, "\"command\":\"SHARDROUTEVALUEDRAFTJSON\"");
    minikv::test_support::assert_contains(
        draft,
        "\"draftMode\":\"controlled-read-only-operator-evidence-value-draft\"");
    minikv::test_support::assert_contains(draft,
                                          "\"sourceImportPreflightCommand\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    minikv::test_support::assert_contains(draft, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    minikv::test_support::assert_contains(
        draft,
        "\"sourceImportPreflightFixturePath\":\"fixtures/release/shard-readiness-v560.json\"");
    minikv::test_support::assert_contains(draft, "\"sourceImportPreflightPublishedStageCount\":25");
    minikv::test_support::assert_contains(draft, "\"sourceImportPreflightChainComplete\":true");
    minikv::test_support::assert_contains(draft, "\"sourceImportPreflightReleaseRangeStart\":\"v536\"");
    minikv::test_support::assert_contains(draft, "\"sourceImportPreflightReleaseRangeEnd\":\"v560\"");
    minikv::test_support::assert_contains(draft, "\"operatorValueDraftSlotCount\":25");
    minikv::test_support::assert_contains(draft, "\"expectedOperatorEvidenceSlotCount\":25");
    minikv::test_support::assert_contains(draft, "\"draftSlotCount\":25");
    minikv::test_support::assert_contains(draft, "\"acceptedOperatorValueCount\":0");
    minikv::test_support::assert_contains(draft, "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(draft, "\"actualValueState\":\"not-supplied\"");
    minikv::test_support::assert_contains(draft, "\"draftValueState\":\"awaiting-operator-value\"");
    minikv::test_support::assert_contains(draft, "\"valueDraftSlotHelperApplied\":true");
    minikv::test_support::assert_contains(draft, "\"draftCheckCount\":16");
    minikv::test_support::assert_contains(draft, "\"draftPassedCount\":16");
    minikv::test_support::assert_contains(draft, "\"readyForOperatorEvidenceValueDraft\":true");
    minikv::test_support::assert_contains(draft, "\"readyForEvidenceImport\":false");
    minikv::test_support::assert_contains(draft, "\"readyForManualEvidenceEntry\":false");
    minikv::test_support::assert_contains(draft, "\"readyForLiveExecution\":false");
    minikv::test_support::assert_contains(draft, "\"readyForProductionExecution\":false");
    minikv::test_support::assert_contains(draft, "\"operatorValueSupplied\":false");
    minikv::test_support::assert_contains(draft, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(draft, "\"operatorValueImported\":false");
    minikv::test_support::assert_contains(draft, "\"operatorEvidenceValuesAccepted\":false");
    minikv::test_support::assert_contains(draft, "\"valueSupplyAdapterInstalled\":false");
    minikv::test_support::assert_contains(draft, "\"valueSupplyAdapterEnabled\":false");
    minikv::test_support::assert_contains(draft, "\"evidenceImportAllowed\":false");
    minikv::test_support::assert_contains(draft, "\"manualEvidenceImportAllowed\":false");
    minikv::test_support::assert_contains(draft, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(draft, "\"syntheticEvidenceAccepted\":false");
    minikv::test_support::assert_contains(draft, "\"syntheticEvidenceRejected\":true");
    minikv::test_support::assert_contains(draft, "\"secretValueStored\":false");
    minikv::test_support::assert_contains(draft, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(draft, "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(draft, "\"normalizerExecutes\":false");
    minikv::test_support::assert_contains(draft, "\"normalizationExecutionAllowed\":false");
    minikv::test_support::assert_contains(draft, "\"redactionPreserved\":true");
    minikv::test_support::assert_contains(draft, "\"draftValuesPersisted\":false");
    minikv::test_support::assert_contains(draft, "\"valueDraftPackageOnly\":true");
    minikv::test_support::assert_contains(draft, "\"singleValueDraftSectionExpected\":true");
    minikv::test_support::assert_contains(draft, "\"draftCode\":\"VALUE_DRAFT_SLOT_TEMPLATE_SPLIT\"");
    minikv::test_support::assert_contains(draft, "\"draftCode\":\"VALUE_DRAFT_RELEASE_PACKAGE\"");
    minikv::test_support::assert_read_only_evidence_boundaries(draft);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewOperatorValueDraft\":") == 1);
    minikv::test_support::assert_contains(shard, "\"status\":\"route-preview-value-draft-");
    minikv::test_support::assert_contains(shard, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    minikv::test_support::assert_contains(shard, "\"readyForOperatorEvidenceValueDraft\":true");
    minikv::test_support::assert_contains(shard, "\"actualValueState\":\"not-supplied\"");
    minikv::test_support::assert_contains(shard, "\"readyForEvidenceImport\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUEDRAFTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUEDRAFTJSON");

    result = processor.execute("SHARDROUTEVALUEDRAFTJSON");
    assert(result.response == draft);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(result.response,
                                          "SHARDROUTEVALUEDRAFTJSON(category=read,mutates_store=no,"
                                          "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(result.response,
                                          "\"name\":\"SHARDROUTEVALUEDRAFTJSON\","
                                          "\"category\":\"read\",\"mutates_store\":false,"
                                          "\"touches_wal\":false");

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEVALUEDRAFTJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEVALUEDRAFTJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, value_draft_command);

    return 0;
}
