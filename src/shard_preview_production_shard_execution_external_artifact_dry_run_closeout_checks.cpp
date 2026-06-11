#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks {
namespace {

constexpr std::array<ProductionShardExecutionExternalArtifactDryRunCloseoutCheck, 12> checks{{
    {1, "source-v1601-mini-kv-owner-receipt-bundle-observed", "v1601-mini-kv-owner-receipt-bundle",
     "external artifact closeout is derived from the latest mini-kv owner receipt evidence"},
    {2, "node-v2093-dry-run-batch-closeout-observed", "node-v2093-dry-run-closeout",
     "mini-kv records the Node dry-run closeout direction without editing Node"},
    {3, "node-only-growth-stop-recorded", "node-only-growth-stop",
     "the dry-run chain stops adding internal-only layers until real external artifacts arrive"},
    {4, "real-external-artifact-missing", "external-artifact",
     "missing real artifact is explicit and cannot be mistaken for accepted evidence"},
    {5, "external-artifact-not-parsed", "external-artifact-parser",
     "mini-kv does not parse or validate raw external artifact payloads"},
    {6, "external-artifact-not-stored", "external-artifact-storage",
     "mini-kv does not store imported artifacts or bind managed audit storage"},
    {7, "dry-run-artifact-not-executed", "external-artifact-execution",
     "dry-run closeout does not execute artifact, approval, rollback, or cleanup behavior"},
    {8, "real-signed-approval-missing", "real-signed-approval",
     "production execution remains blocked without a real signed approval artifact"},
    {9, "managed-audit-owner-binding-missing", "managed-audit-owner-binding",
     "managed audit owner binding is still missing and no raw endpoint or credential is read"},
    {10, "sibling-real-receipts-missing", "java-mini-kv-cleanup-real-receipts",
     "Java, real mini-kv signed, and cleanup receipts remain missing as real external blockers"},
    {11, "production-authority-remains-closed", "production-shard-execution-authority",
     "closeout evidence does not open production shard execution, routing, writes, WAL, or services"},
    {12, "f-root-per-version-archive-required", "f-root-command-archive",
     "each v1602-v1606 release writes screenshots and explanations under its own f-root version directory"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const ProductionShardExecutionExternalArtifactDryRunCloseoutCheck>
production_shard_execution_external_artifact_dry_run_closeout_checks() {
    return checks;
}

int planned_production_shard_execution_external_artifact_dry_run_closeout_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_production_shard_execution_external_artifact_dry_run_closeout_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds external artifact dry-run closeout checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"artifactSlot\":" + json_string(check.artifact_slot) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"dryRunCloseoutEvidence\":true"
                  ",\"realExternalArtifactPresent\":false"
                  ",\"externalArtifactParsed\":false"
                  ",\"externalArtifactAccepted\":false"
                  ",\"externalArtifactStored\":false"
                  ",\"dryRunArtifactExecuted\":false"
                  ",\"realSignedApprovalPresent\":false"
                  ",\"managedAuditOwnerBindingPresent\":false"
                  ",\"realJavaReceiptPresent\":false"
                  ",\"realMiniKvSignedReceiptPresent\":false"
                  ",\"realCleanupReceiptPresent\":false"
                  ",\"productionShardExecutionEnabled\":false"
                  ",\"managedAuditStoreBound\":false"
                  ",\"rawEndpointParsed\":false"
                  ",\"credentialValueRead\":false"
                  ",\"startsNodeServices\":false"
                  ",\"startsJavaService\":false"
                  ",\"startsMiniKvService\":false"
                  ",\"activeRouterInstalled\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"mutatesStore\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks
