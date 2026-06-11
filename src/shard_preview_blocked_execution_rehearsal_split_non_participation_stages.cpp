#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_stages.hpp"

#include <array>

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, 20> stages{{
    {1, "v1506", "route-preview-blocked-execution-rehearsal-split-non-participation-entrypoint-boundary",
     "v1505", "fixtures/release/shard-readiness-v1505.json",
     "records Node v1968 public loader preservation without importing the service"},
    {2, "v1507", "route-preview-blocked-execution-rehearsal-split-non-participation-types-boundary",
     "v1506", "fixtures/release/shard-readiness-v1506.json",
     "records Node v1969 profile reference evidence attempt check message and receipt type extraction"},
    {3, "v1508", "route-preview-blocked-execution-rehearsal-split-non-participation-constants-boundary",
     "v1507", "fixtures/release/shard-readiness-v1507.json",
     "records Node v1970 path endpoint digest and accepted mini-kv reference constants extraction"},
    {4, "v1509", "route-preview-blocked-execution-rehearsal-split-non-participation-historical-reference-boundary",
     "v1508", "fixtures/release/shard-readiness-v1508.json",
     "records Node v1971 historical evidence file and snippet collection extraction without mini-kv file parsing"},
    {5, "v1510", "route-preview-blocked-execution-rehearsal-split-non-participation-java-v90-reference-boundary",
     "v1509", "fixtures/release/shard-readiness-v1509.json",
     "records Node v1972 Java v90 context-normalization projection as read-only metadata"},
    {6, "v1511", "route-preview-blocked-execution-rehearsal-split-non-participation-minikv-v99-reference-boundary",
     "v1510", "fixtures/release/shard-readiness-v1510.json",
     "records Node v1973 mini-kv v99 WAL regression projection as read-only metadata"},
    {7, "v1512", "route-preview-blocked-execution-rehearsal-split-non-participation-renderer-boundary",
     "v1511", "fixtures/release/shard-readiness-v1511.json",
     "records Node v1974 Markdown renderer extraction without renderer execution in mini-kv"},
    {8, "v1513", "route-preview-blocked-execution-rehearsal-split-non-participation-digest-core-boundary",
     "v1512", "fixtures/release/shard-readiness-v1512.json",
     "records Node v1975 digest assembly extraction into core without C++ execution"},
    {9, "v1514", "route-preview-blocked-execution-rehearsal-split-non-participation-summary-core-boundary",
     "v1513", "fixtures/release/shard-readiness-v1513.json",
     "records Node v1976 summary aggregation extraction into core without C++ execution"},
    {10, "v1515", "route-preview-blocked-execution-rehearsal-split-non-participation-attempt-policy-boundary",
     "v1514", "fixtures/release/shard-readiness-v1514.json",
     "records Node v1977 simulated blocked attempts and readiness checks as Node-owned policy"},
    {11, "v1516", "route-preview-blocked-execution-rehearsal-split-non-participation-message-policy-boundary",
     "v1515", "fixtures/release/shard-readiness-v1515.json",
     "records Node v1978 blocker warning and recommendation messages as Node-owned policy"},
    {12, "v1517", "route-preview-blocked-execution-rehearsal-split-non-participation-focused-route-tests-observed",
     "v1516", "fixtures/release/shard-readiness-v1516.json",
     "records Node v1979 focused behavior and route output tests as observed sibling checks"},
    {13, "v1518", "route-preview-blocked-execution-rehearsal-split-non-participation-downstream-intake-observed",
     "v1517", "fixtures/release/shard-readiness-v1517.json",
     "records Node v1980 downstream precondition intake consumption tests as observed sibling checks"},
    {14, "v1519", "route-preview-blocked-execution-rehearsal-split-non-participation-typecheck-build-observed",
     "v1518", "fixtures/release/shard-readiness-v1518.json",
     "records Node v1981 typecheck and build as observed sibling checks"},
    {15, "v1520", "route-preview-blocked-execution-rehearsal-split-non-participation-node-closeout-observed",
     "v1519", "fixtures/release/shard-readiness-v1519.json",
     "records Node v1982 archive cleanup commit tag push and CI as Node-owned closeout"},
    {16, "v1521", "route-preview-blocked-execution-rehearsal-split-non-participation-minikv-v99-non-parse-lock",
     "v1520", "fixtures/release/shard-readiness-v1520.json",
     "locks historical mini-kv v99 WAL regression as referenced metadata rather than parsed runtime evidence"},
    {17, "v1522", "route-preview-blocked-execution-rehearsal-split-non-participation-java-v90-non-scan-lock",
     "v1521", "fixtures/release/shard-readiness-v1521.json",
     "locks Java v90 context-normalization evidence as referenced metadata rather than scanned sibling files"},
    {18, "v1523", "route-preview-blocked-execution-rehearsal-split-non-participation-downstream-no-execution-lock",
     "v1522", "fixtures/release/shard-readiness-v1522.json",
     "keeps downstream precondition intake consumption outside mini-kv execution authority"},
    {19, "v1524", "route-preview-blocked-execution-rehearsal-split-non-participation-command-fixture-smoke-boundary",
     "v1523", "fixtures/release/shard-readiness-v1523.json",
     "closes command fixture and smoke expectations without service startup or WAL touch"},
    {20, "v1525", "route-preview-blocked-execution-rehearsal-split-non-participation-clean-workspace-ci-closeout",
     "v1524", "fixtures/release/shard-readiness-v1524.json",
     "closes mini-kv evidence with archive cleanup commit tag push and CI verification"},
}};

} // namespace

std::span<const StageRecord> blocked_execution_rehearsal_split_non_participation_stages() {
    return stages;
}

int first_blocked_execution_rehearsal_split_non_participation_release_number() {
    return 1506;
}

int planned_blocked_execution_rehearsal_split_non_participation_stage_count() {
    return static_cast<int>(stages.size());
}

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_stages
