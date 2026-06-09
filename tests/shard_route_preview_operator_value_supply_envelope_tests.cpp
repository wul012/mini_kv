#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view value_supply_command = "SHARDROUTEVALUESUPPLYJSON";

} // namespace

int main() {
    const std::string envelope =
        minikv::shard_route_preview_operator_value_supply_envelope::format_value_supply_envelope_json();
    minikv::test_support::assert_contains(envelope,
                                          "\"contract\":\"shard-route-preview-operator-value-supply-envelope.v1\"");
    minikv::test_support::assert_contains(envelope, "\"command\":\"SHARDROUTEVALUESUPPLYJSON\"");
    minikv::test_support::assert_contains(
        envelope,
        "\"envelopeMode\":\"controlled-read-only-disabled-operator-value-supply-envelope\"");
    minikv::test_support::assert_contains(envelope, "\"sourceValueDraftCommand\":\"SHARDROUTEVALUEDRAFTJSON\"");
    minikv::test_support::assert_contains(envelope, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    minikv::test_support::assert_contains(
        envelope,
        "\"sourceValueDraftFixturePath\":\"fixtures/release/shard-readiness-v585.json\"");
    minikv::test_support::assert_contains(envelope, "\"sourceValueDraftPublishedStageCount\":25");
    minikv::test_support::assert_contains(envelope, "\"sourceValueDraftChainComplete\":true");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyReleaseRangeStart\":\"v586\"");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyReleaseRangeEnd\":\"v610\"");
    minikv::test_support::assert_contains(envelope, "\"operatorValueEnvelopeSlotCount\":25");
    minikv::test_support::assert_contains(envelope, "\"expectedOperatorEvidenceSlotCount\":25");
    minikv::test_support::assert_contains(envelope, "\"envelopeSlotCount\":25");
    minikv::test_support::assert_contains(envelope, "\"suppliedOperatorValueCount\":0");
    minikv::test_support::assert_contains(envelope, "\"acceptedOperatorValueCount\":0");
    minikv::test_support::assert_contains(envelope, "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(envelope, "\"actualValueState\":\"not-supplied\"");
    minikv::test_support::assert_contains(envelope,
                                          "\"operatorValueEnvelopeState\":\"disabled-by-default\"");
    minikv::test_support::assert_contains(envelope, "\"missingValuePolicy\":\"fail-closed\"");
    minikv::test_support::assert_contains(envelope, "\"redactionPolicy\":\"redact-before-persist\"");
    minikv::test_support::assert_contains(envelope, "\"valueSupplySlotHelperApplied\":true");
    minikv::test_support::assert_contains(envelope, "\"envelopeCheckCount\":18");
    minikv::test_support::assert_contains(envelope, "\"envelopePassedCount\":18");
    minikv::test_support::assert_contains(envelope, "\"readyForDisabledValueSupplyEnvelope\":true");
    minikv::test_support::assert_contains(envelope, "\"readyForOperatorValueSupply\":false");
    minikv::test_support::assert_contains(envelope, "\"readyForEvidenceImport\":false");
    minikv::test_support::assert_contains(envelope, "\"readyForManualEvidenceEntry\":false");
    minikv::test_support::assert_contains(envelope, "\"readyForRuntimePayloadAcceptance\":false");
    minikv::test_support::assert_contains(envelope, "\"readyForLiveExecution\":false");
    minikv::test_support::assert_contains(envelope, "\"readyForProductionExecution\":false");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyEnvelopeDeclared\":true");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyEnvelopeDisabledByDefault\":true");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyAdapterInstalled\":false");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyAdapterEnabled\":false");
    minikv::test_support::assert_contains(envelope, "\"operatorValueSupplied\":false");
    minikv::test_support::assert_contains(envelope, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(envelope, "\"operatorValueImported\":false");
    minikv::test_support::assert_contains(envelope, "\"operatorEvidenceValuesAccepted\":false");
    minikv::test_support::assert_contains(envelope, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(envelope, "\"sourceEvidenceProvenanceRequired\":true");
    minikv::test_support::assert_contains(envelope, "\"sourceEvidenceProvenanceSupplied\":false");
    minikv::test_support::assert_contains(envelope, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(envelope, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(envelope, "\"syntheticEvidenceAccepted\":false");
    minikv::test_support::assert_contains(envelope, "\"syntheticEvidenceRejected\":true");
    minikv::test_support::assert_contains(envelope, "\"secretValueStored\":false");
    minikv::test_support::assert_contains(envelope, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(envelope, "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(envelope, "\"redactionPolicyDeclared\":true");
    minikv::test_support::assert_contains(envelope, "\"redactionPolicyPreviewOnly\":true");
    minikv::test_support::assert_contains(envelope, "\"normalizerExecutes\":false");
    minikv::test_support::assert_contains(envelope, "\"normalizationExecutionAllowed\":false");
    minikv::test_support::assert_contains(envelope, "\"envelopeValuesPersisted\":false");
    minikv::test_support::assert_contains(envelope, "\"valueSupplyEnvelopePackageOnly\":true");
    minikv::test_support::assert_contains(envelope, "\"singleValueSupplyEnvelopeSectionExpected\":true");
    minikv::test_support::assert_contains(envelope,
                                          "\"envelopeCode\":\"VALUE_SUPPLY_ENVELOPE_POLICY_SPLIT\"");
    minikv::test_support::assert_contains(envelope, "\"envelopeCode\":\"VALUE_SUPPLY_RELEASE_PACKAGE\"");
    minikv::test_support::assert_read_only_evidence_boundaries(envelope);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewOperatorValueSupplyEnvelope\":") == 1);
    minikv::test_support::assert_contains(shard,
                                          "\"status\":\"route-preview-candidate-material-submission-precheck-integrity-closeout-summary-read-only\"");
    minikv::test_support::assert_contains(shard, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    minikv::test_support::assert_contains(shard, "\"readyForDisabledValueSupplyEnvelope\":true");
    minikv::test_support::assert_contains(shard, "\"readyForOperatorValueSupply\":false");
    minikv::test_support::assert_contains(shard, "\"actualValueState\":\"not-supplied\"");
    minikv::test_support::assert_contains(shard, "\"readyForEvidenceImport\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYJSON");
    assert(result.response == envelope);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(result.response,
                                          "SHARDROUTEVALUESUPPLYJSON(category=read,mutates_store=no,"
                                          "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(result.response,
                                          "\"name\":\"SHARDROUTEVALUESUPPLYJSON\","
                                          "\"category\":\"read\",\"mutates_store\":false,"
                                          "\"touches_wal\":false");

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, value_supply_command);

    return 0;
}
