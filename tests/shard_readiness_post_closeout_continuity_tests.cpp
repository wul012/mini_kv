#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const std::string json = minikv::shard_readiness::format_json();

    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\",\"releaseVersion\":\"v407\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v360\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v360.json\"");
    assert_contains(json,
                    "\"continuityStage\":\"feature-read-only-shard-route-preview-verification-report-archive-closeout-release-package\"");
    assert_contains(json, "\"stageSequence\":180");
    assert_contains(json, "\"stageReleaseVersion\":\"v380\"");
    assert_contains(json, "\"trackedPostCloseoutRangeEnd\":\"v380\"");
    assert_contains(json, "\"trackedPostCloseoutReleaseCount\":180");
    assert_contains(json, "\"nodeBatchCloseoutVersion\":\"Node v549\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:01e736bc2963baae\"");
    assert_contains(json, "\"archiveVerifierCheckCount\":3");
    assert_contains(json, "\"archiveVerifierChecksPassed\":3");
    assert_contains(json, "\"stage descriptor lifecycle preserves the JSON contract\"");
    assert_contains(json, "\"no router, write routing, or execution authority is opened\"");
    assert_contains(json, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"fixtures/release/shard-readiness-v232.json\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    const auto result = processor.execute("SHARDJSON");

    assert(result.response == json);
    assert(store.size() == 0);
    assert_not_contains(result.response, "\"executionAllowed\":true");
    assert_not_contains(result.response, "\"routerActivationAllowed\":true");
    assert_not_contains(result.response, "\"writeRoutingAllowed\":true");
}
