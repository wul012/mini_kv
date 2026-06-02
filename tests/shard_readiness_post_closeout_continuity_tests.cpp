#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

void assert_not_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) == std::string::npos);
}

} // namespace

int main() {
    const std::string json = minikv::shard_readiness::format_json();

    assert_contains(json, "\"releaseVersion\":\"v234\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v522-post-java-mini-kv-route-catalog-cleanup-twenty-version-run-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v233\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v233.json\"");
    assert_contains(json, "\"continuityStage\":\"post-closeout-continuity-node-v522-twenty-version-run-closeout-alignment-audit\"");
    assert_contains(json, "\"stageSequence\":34");
    assert_contains(json, "\"stageReleaseVersion\":\"v234\"");
    assert_contains(json, "\"trackedPostCloseoutRangeEnd\":\"v234\"");
    assert_contains(json, "\"trackedPostCloseoutReleaseCount\":34");
    assert_contains(json, "\"nodeBatchCloseoutVersion\":\"Node v522\"");
    assert_contains(json, "\"archiveVerifierCheckCount\":10");
    assert_contains(json, "\"archiveVerifierChecksPassed\":10");
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
