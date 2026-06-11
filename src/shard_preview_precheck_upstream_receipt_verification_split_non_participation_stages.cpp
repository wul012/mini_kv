#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 20> stages{{
    {1, "v1526", "route-preview-precheck-upstream-receipt-verification-split-non-participation-entrypoint-export-boundary",
     "v1525", "fixtures/release/shard-readiness-v1525.json",
     "records Node v1983 public loader and render export preservation without importing the service"},
    {2, "v1527", "route-preview-precheck-upstream-receipt-verification-split-non-participation-types-boundary",
     "v1526", "fixtures/release/shard-readiness-v1526.json",
     "records Node v1984 profile receipt evidence snippet message and check type extraction"},
    {3, "v1528", "route-preview-precheck-upstream-receipt-verification-split-non-participation-constants-boundary",
     "v1527", "fixtures/release/shard-readiness-v1527.json",
     "records Node v1985 fixed evidence paths endpoint paths and precheck shape constants extraction"},
    {4, "v1529", "route-preview-precheck-upstream-receipt-verification-split-non-participation-node-v245-reference-boundary",
     "v1528", "fixtures/release/shard-readiness-v1528.json",
     "records Node v1986 Node v245 source adapter reference as Node-owned metadata"},
    {5, "v1530", "route-preview-precheck-upstream-receipt-verification-split-non-participation-java-v99-reference-boundary",
     "v1529", "fixtures/release/shard-readiness-v1529.json",
     "records Node v1987 Java v99 receipt reference builder without Java file scanning"},
    {6, "v1531", "route-preview-precheck-upstream-receipt-verification-split-non-participation-minikv-v108-reference-boundary",
     "v1530", "fixtures/release/shard-readiness-v1530.json",
     "records Node v1988 mini-kv v108 non-participation reference builder without historical file parsing"},
    {7, "v1532", "route-preview-precheck-upstream-receipt-verification-split-non-participation-reference-helper-private-boundary",
     "v1531", "fixtures/release/shard-readiness-v1531.json",
     "records Node v1989 private evidence-file snippet JSON and field helpers without C++ execution"},
    {8, "v1533", "route-preview-precheck-upstream-receipt-verification-split-non-participation-check-policy-boundary",
     "v1532", "fixtures/release/shard-readiness-v1532.json",
     "records Node v1990 check construction policy extraction as Node-owned behavior"},
    {9, "v1534", "route-preview-precheck-upstream-receipt-verification-split-non-participation-message-policy-boundary",
     "v1533", "fixtures/release/shard-readiness-v1533.json",
     "records Node v1991 blocker warning and recommendation policy extraction as Node-owned behavior"},
    {10, "v1535", "route-preview-precheck-upstream-receipt-verification-split-non-participation-core-assembly-boundary",
     "v1534", "fixtures/release/shard-readiness-v1534.json",
     "records Node v1992 digest receipt verification summary endpoint and next-action assembly as Node-owned core"},
    {11, "v1536", "route-preview-precheck-upstream-receipt-verification-split-non-participation-renderer-boundary",
     "v1535", "fixtures/release/shard-readiness-v1535.json",
     "records Node v1993 Markdown rendering extraction without renderer execution in mini-kv"},
    {12, "v1537", "route-preview-precheck-upstream-receipt-verification-split-non-participation-entrypoint-orchestration-boundary",
     "v1536", "fixtures/release/shard-readiness-v1536.json",
     "records Node v1994 entrypoint orchestration-only closeout and import compatibility preservation"},
    {13, "v1538", "route-preview-precheck-upstream-receipt-verification-split-non-participation-code-health-scan-family-observed",
     "v1537", "fixtures/release/shard-readiness-v1537.json",
     "records Node v1995 code-health scan update from one service file to the module family"},
    {14, "v1539", "route-preview-precheck-upstream-receipt-verification-split-non-participation-large-file-inventory-observed",
     "v1538", "fixtures/release/shard-readiness-v1538.json",
     "records Node v1996 large-file inventory cleanup as observed sibling metadata"},
    {15, "v1540", "route-preview-precheck-upstream-receipt-verification-split-non-participation-direct-tests-fallback-observed",
     "v1539", "fixtures/release/shard-readiness-v1539.json",
     "records Node v1997 direct service tests and forced historical fixture fallback as observed sibling checks"},
    {16, "v1541", "route-preview-precheck-upstream-receipt-verification-split-non-participation-downstream-code-health-observed",
     "v1540", "fixtures/release/shard-readiness-v1540.json",
     "records Node v1998 downstream code-health behavior as observed sibling checks"},
    {17, "v1542", "route-preview-precheck-upstream-receipt-verification-split-non-participation-downstream-rehearsal-guard-observed",
     "v1541", "fixtures/release/shard-readiness-v1541.json",
     "records Node v1999 downstream rehearsal guard ready-state preservation as observed sibling checks"},
    {18, "v1543", "route-preview-precheck-upstream-receipt-verification-split-non-participation-typecheck-build-observed",
     "v1542", "fixtures/release/shard-readiness-v1542.json",
     "records Node v2000 TypeScript typecheck and production build as observed sibling checks"},
    {19, "v1544", "route-preview-precheck-upstream-receipt-verification-split-non-participation-node-closeout-observed",
     "v1543", "fixtures/release/shard-readiness-v1543.json",
     "records Node v2001 commit tag push and CI as Node-owned closeout"},
    {20, "v1545", "route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout",
     "v1544", "fixtures/release/shard-readiness-v1544.json",
     "closes mini-kv evidence with cleanup and no local long-running process proof"},
}};

} // namespace

std::span<const StageRecord> precheck_upstream_receipt_verification_split_non_participation_stages() {
    return stages;
}

int first_precheck_upstream_receipt_verification_split_non_participation_release_number() {
    return 1526;
}

int planned_precheck_upstream_receipt_verification_split_non_participation_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages
