#include "minikv/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation;

    const auto passed =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                15, true, false, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(
        passed,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false");
    assert_contains(passed, "\"endpointHandleRead\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"credentialValueRead\":false");
    assert_contains(passed, "\"managedAuditConnectionOpened\":false");
    assert_contains(passed, "\"activeRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                14, true, false, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(
        failed_source,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_fresh =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                15, true, true, false, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(
        failed_fresh,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_fresh, "\"nodePlanObservedOnly\":false");

    const auto failed_import =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                15, true, false, true, false, false, false, false, false, 15, 15, 15, 15);
    assert_contains(
        failed_import,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_import, "\"miniKvBoundaryClosed\":false");

    const auto failed_receipt_parse =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                15, true, false, false, false, false, false, false, true, 15, 15, 15, 15);
    assert_contains(
        failed_receipt_parse,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_receipt_parse, "\"miniKvBoundaryClosed\":false");

    const auto failed_count =
        validation::
            format_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_json(
                15, true, false, false, false, false, false, false, false, 15, 14, 15, 15);
    assert_contains(
        failed_count,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
