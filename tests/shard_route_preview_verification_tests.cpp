#include "minikv/command.hpp"
#include "minikv/shard_route_preview_verification.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const auto alpha = minikv::shard_route_preview_verification::verify_key("orderops:alpha");
    assert(alpha.preview.slot == 5);
    assert(alpha.slot_within_range);
    assert(alpha.single_shard);
    assert(alpha.preview_only);
    assert(alpha.pinned_sample_consistent);
    assert(alpha.hash_present);
    assert(alpha.read_only);
    assert(alpha.no_active_router);
    assert(alpha.no_write_routing);
    assert(alpha.no_execution);
    assert(alpha.check_count == 9);
    assert(alpha.passed_count == 9);

    const auto custom = minikv::shard_route_preview_verification::verify_key("customer:42");
    assert(custom.slot_within_range);
    assert(custom.pinned_sample_consistent);
    assert(custom.passed_count == custom.check_count);

    const std::string verification_json =
        minikv::shard_route_preview_verification::format_verification_json("orderops:alpha");
    assert_contains(verification_json, "\"contract\":\"shard-route-preview-verification.v1\"");
    assert_contains(verification_json, "\"command\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(verification_json, "\"checkedCommand\":\"SHARDROUTEJSON\"");
    assert_contains(verification_json, "\"verificationMode\":\"read-only-route-preview-field-check\"");
    assert_contains(verification_json, "\"key\":\"orderops:alpha\"");
    assert_contains(verification_json, "\"slot\":5");
    assert_contains(verification_json, "\"passed\":true");
    assert_contains(verification_json, "\"checkCount\":9");
    assert_contains(verification_json, "\"passedCount\":9");
    assert_contains(verification_json, "\"failedCount\":0");
    assert_contains(verification_json, "\"name\":\"slot_within_range\",\"passed\":true");
    assert_contains(verification_json, "\"name\":\"no_execution\",\"passed\":true");
    assert_contains(verification_json, "\"rolloutStage\":\"route-preview-verification-cli-smoke-contract\"");
    assert_contains(verification_json, "\"rolloutStageSequence\":5");
    assert_contains(verification_json, "\"rolloutReleaseVersion\":\"v303\"");
    assert_contains(verification_json, "\"readOnly\":true");
    assert_contains(verification_json, "\"mutatesStore\":false");
    assert_contains(verification_json, "\"touchesWal\":false");
    assert_contains(verification_json, "\"executesRoute\":false");
    assert_contains(verification_json, "\"activeRouterInstalled\":false");
    assert_contains(verification_json, "\"writeRoutingAllowed\":false");
    assert_contains(verification_json, "\"executionAllowed\":false");

    const std::string rollout = minikv::shard_route_preview_verification::format_rollout_json();
    assert_contains(rollout, "\"rolloutMode\":\"read-only-shard-route-preview-verification-rollout\"");
    assert_contains(rollout, "\"sourceFrozenReleaseVersion\":\"v302\"");
    assert_contains(rollout, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v302.json\"");
    assert_contains(rollout, "\"rolloutStage\":\"route-preview-verification-cli-smoke-contract\"");
    assert_contains(rollout, "\"rolloutStageSequence\":5");
    assert_contains(rollout, "\"rolloutReleaseVersion\":\"v303\"");
    assert_contains(rollout, "\"publishedStageCount\":5");
    assert_contains(rollout, "\"verificationCommandAvailable\":true");
    assert_contains(rollout, "\"verifiedCommand\":\"SHARDROUTEJSON\"");
    assert_contains(rollout, "\"commands\":[\"SHARDROUTEVERIFYJSON\"]");
    assert_contains(rollout, "\"activeRouterInstalled\":false");
    assert_contains(rollout, "\"writeRoutingAllowed\":false");
    assert_contains(rollout, "\"executionAllowed\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVERIFYJSON");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYJSON key");

    result = processor.execute("SHARDROUTEVERIFYJSON orderops:alpha extra");
    assert(result.response == "ERR usage: SHARDROUTEVERIFYJSON key");

    result = processor.execute("SHARDROUTEVERIFYJSON orderops:alpha");
    assert(result.response == verification_json);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEVERIFYJSON orderops:alpha");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"key\":\"orderops:alpha\"");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEVERIFYJSON orderops:alpha");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"not a write command\"");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
