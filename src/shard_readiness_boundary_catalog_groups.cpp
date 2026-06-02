#include "shard_readiness_boundary_catalog_groups.hpp"

namespace minikv::shard_readiness::boundary_fields {

const std::vector<std::string>& boundary_catalog_groups() {
    static const std::vector<std::string> groups = {
        "top-level-read-only",
        "slot-table-preview",
        "slot-table-preview-audit",
        "slot-table-preview-audit-maintenance",
        "shard-readiness-release-catalog",
        "shard-readiness-release-catalog-audit",
        "core-boundaries",
        "historical-fallback",
        "shard-readiness-history-maintenance",
        "active-prototype-plan",
        "consumer-handoff",
        "live-read-gate",
        "operator-service-lifecycle",
        "runtime-execution-artifacts",
        "approval-gate-inputs",
        "final-mini-kv-approval-input",
        "template-validator-echo",
        "canonical-approval-input-precheck",
        "node-route-group-split-compatibility",
        "node-route-split-compatibility-window",
        "node-route-split-compatibility-window-audit",
        "node-route-catalog-closeout-snapshot",
        "node-route-catalog-anchor-removal-audit",
        "node-route-catalog-expected-integrity-snapshot-audit",
        "node-route-catalog-expected-integrity-snapshot-freeze",
        "node-route-catalog-evidence-chain",
        "node-route-catalog-evidence-chain-audit",
        "node-route-catalog-cleanup-closeout-handoff",
        "node-route-catalog-cleanup-closeout-handoff-audit",
        "node-route-catalog-cleanup-closeout-handoff-audit-freeze",
        "node-route-catalog-cleanup-closeout-release-catalog",
        "node-route-catalog-cleanup-closeout-release-catalog-audit",
        "node-route-catalog-cleanup-closeout-catalog-maintenance",
        "node-route-catalog-cleanup-latest-evidence-package",
        "node-route-catalog-cleanup-latest-evidence-package-audit",
        "node-route-catalog-cleanup-evidence-batch-closeout",
        "node-route-catalog-cleanup-evidence-batch-closeout-audit",
        "node-route-catalog-cleanup-post-closeout-continuity",
        "boundary-catalog-maintenance",
        "boundary-catalog-index",
    };
    return groups;
}

} // namespace minikv::shard_readiness::boundary_fields
