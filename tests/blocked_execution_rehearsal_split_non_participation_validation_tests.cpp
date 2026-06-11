#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_validation;

    const auto passed =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(passed, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"nodePlanObservedOnly\":true");
    assert_contains(passed, "\"miniKvBoundaryClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
    assert_contains(passed, "\"nodePreconditionIntakeExecutedByMiniKv\":false");
    assert_contains(passed, "\"miniKvImportsNodeModules\":false");
    assert_contains(passed, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(passed, "\"miniKvStartsServices\":false");
    assert_contains(passed, "\"miniKvReadsEndpoints\":false");
    assert_contains(passed, "\"miniKvReadsCredentials\":false");
    assert_contains(passed, "\"miniKvParsesHistoricalReceipts\":false");
    assert_contains(passed, "\"miniKvScansJavaEvidence\":false");
    assert_contains(passed, "\"miniKvReplaysWalRegression\":false");
    assert_contains(passed, "\"endpointHandleRead\":false");
    assert_contains(passed, "\"rawEndpointParsed\":false");
    assert_contains(passed, "\"credentialValueRead\":false");
    assert_contains(passed, "\"managedAuditConnectionOpened\":false");
    assert_contains(passed, "\"activeRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source_count =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            14, true, false, false, false, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_source_count,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_source_count, "\"sourceLocked\":false");

    const auto failed_source_complete =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, false, false, false, false, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_source_complete,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_source_complete, "\"sourceLocked\":false");

    const auto failed_fresh =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, true, false, false, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_fresh, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_fresh, "\"nodePlanObservedOnly\":false");

    const auto failed_import =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, true, false, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_import, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_import, "\"miniKvBoundaryClosed\":false");

    const auto failed_node_checks =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, true, false, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_node_checks,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_node_checks, "\"miniKvBoundaryClosed\":false");

    const auto failed_service =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, true, false, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_service, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_service, "\"miniKvBoundaryClosed\":false");

    const auto failed_endpoint =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, true, false, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_endpoint, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_endpoint, "\"miniKvBoundaryClosed\":false");

    const auto failed_credential =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, true, false, false, false, 20, 20, 20,
            20);
    assert_contains(failed_credential,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_credential, "\"miniKvBoundaryClosed\":false");

    const auto failed_receipt_parse =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, true, false, false, 20, 20, 20,
            20);
    assert_contains(failed_receipt_parse,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_receipt_parse, "\"miniKvBoundaryClosed\":false");

    const auto failed_java_scan =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, false, true, false, 20, 20, 20,
            20);
    assert_contains(failed_java_scan,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_java_scan, "\"miniKvBoundaryClosed\":false");

    const auto failed_wal_replay =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, false, false, true, 20, 20, 20,
            20);
    assert_contains(failed_wal_replay,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_wal_replay, "\"miniKvBoundaryClosed\":false");

    const auto failed_counts =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, false, false, false, 20, 19, 20,
            20);
    assert_contains(failed_counts, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_counts, "\"countsAligned\":false");

    const auto failed_published =
        validation::format_blocked_execution_rehearsal_split_non_participation_validation_json(
            15, true, false, false, false, false, false, false, false, false, false, 20, 20, 19,
            20);
    assert_contains(failed_published,
                    "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_published, "\"countsAligned\":false");

    return 0;
}
