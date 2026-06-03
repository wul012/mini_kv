#include "minikv/command.hpp"
#include "minikv/shard_route_preview.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::assert_not_contains;

} // namespace

int main() {
    const auto alpha = minikv::shard_route_preview::preview_key("orderops:alpha");
    assert(alpha.slot == 5);
    assert(alpha.shard_id == "shard-0");
    assert(alpha.route_mode == "preview-only");
    assert(alpha.pinned_fixture_sample);

    const auto audit = minikv::shard_route_preview::preview_key("audit:receipt:001");
    assert(audit.slot == 9);
    assert(audit.pinned_fixture_sample);

    const auto health = minikv::shard_route_preview::preview_key("mini-kv:health");
    assert(health.slot == 14);
    assert(health.pinned_fixture_sample);

    const auto custom = minikv::shard_route_preview::preview_key("customer:42");
    assert(custom.slot >= 0);
    assert(custom.slot < 16);
    assert(!custom.pinned_fixture_sample);
    assert_contains(custom.preview_hash, "fnv1a64:");

    const std::string route_json = minikv::shard_route_preview::format_route_json("orderops:alpha");
    assert_contains(route_json, "\"contract\":\"shard-route-preview.v1\"");
    assert_contains(route_json, "\"command\":\"SHARDROUTEJSON\"");
    assert_contains(route_json, "\"key\":\"orderops:alpha\"");
    assert_contains(route_json, "\"slot\":5");
    assert_contains(route_json, "\"slotCount\":16");
    assert_contains(route_json, "\"shardId\":\"shard-0\"");
    assert_contains(route_json, "\"routeMode\":\"preview-only\"");
    assert_contains(route_json, "\"pinnedFixtureSample\":true");
    assert_contains(route_json, "\"readOnly\":true");
    assert_contains(route_json, "\"mutatesStore\":false");
    assert_contains(route_json, "\"touchesWal\":false");
    assert_contains(route_json, "\"activeRouterInstalled\":false");
    assert_contains(route_json, "\"writeRoutingAllowed\":false");
    assert_contains(route_json, "\"executionAllowed\":false");

    const std::string rollout = minikv::shard_route_preview::format_rollout_json();
    assert_contains(rollout, "\"rolloutMode\":\"read-only-shard-route-preview-rollout\"");
    assert_contains(rollout, "\"sourceFrozenReleaseVersion\":\"v282\"");
    assert_contains(rollout, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v282.json\"");
    assert_contains(rollout, "\"rolloutStage\":\"route-preview-cli-smoke-contract\"");
    assert_contains(rollout, "\"rolloutStageSequence\":5");
    assert_contains(rollout, "\"rolloutReleaseVersion\":\"v283\"");
    assert_contains(rollout, "\"publishedStageCount\":5");
    assert_contains(rollout, "\"commands\":[\"SHARDROUTE\",\"SHARDROUTEJSON\"]");
    assert_contains(rollout, "\"sampleSlotsPinnedToExistingEvidence\":true");
    assert_contains(rollout, "\"activeRouterInstalled\":false");
    assert_contains(rollout, "\"writeRoutingAllowed\":false");
    assert_contains(rollout, "\"executionAllowed\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTE");
    assert(result.response == "ERR usage: SHARDROUTE key");

    result = processor.execute("SHARDROUTE orderops:alpha extra");
    assert(result.response == "ERR usage: SHARDROUTE key");

    result = processor.execute("SHARDROUTE orderops:alpha");
    assert(result.response == "slot=5 shard=shard-0 route_mode=preview-only read_only=yes "
                              "router_installed=no write_routing_allowed=no execution_allowed=no");
    assert(store.size() == 0);

    result = processor.execute("SHARDROUTEJSON orderops:alpha");
    assert(result.response == route_json);
    assert(store.size() == 0);

    result = processor.execute("EXPLAINJSON SHARDROUTEJSON orderops:alpha");
    assert_contains(result.response, "\"command\":\"SHARDROUTEJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"key\":\"orderops:alpha\"");
    assert_contains(result.response, "\"allowed_by_parser\":true");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDROUTEJSON orderops:alpha");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SHARDROUTEJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"not a write command\"");
    assert_not_contains(result.response, "\"execution_allowed\":true");

    return 0;
}
