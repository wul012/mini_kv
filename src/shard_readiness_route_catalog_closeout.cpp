#include "minikv/shard_readiness_route_catalog_closeout.hpp"

#include <string>

namespace minikv::shard_readiness::route_catalog_closeout {

std::string format_closeout_snapshot_json() {
    return "{\"snapshotMode\":\"node-route-catalog-final-closeout-snapshot-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v464-post-audit-route-catalog-final-closeout-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v184\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\","
           "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\","
           "\"nodeFinalCloseoutVersion\":\"Node v464\","
           "\"routeCatalogBatchStart\":\"Node v433\","
           "\"routeCatalogBatchEnd\":\"Node v464\","
           "\"coveredNodeVersionCount\":32,"
           "\"focusedCloseoutTestFileCount\":7,"
           "\"focusedCloseoutTestCount\":17,"
           "\"nodeTypecheckPassed\":true,"
           "\"nodeBuildPassed\":true,"
           "\"nodeFullVitestFileCount\":393,"
           "\"nodeFullVitestTestCount\":1221,"
           "\"nodeConsumesFreshMiniKvEvidence\":false,"
           "\"miniKvRuntimeContractChanged\":false,"
           "\"snapshotOnly\":true,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::route_catalog_closeout
