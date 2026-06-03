#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-command-explain-contract",
    81,
    "v281",
    "freezes EXPLAINJSON route preview semantics as metadata read only",
    "v280",
    "fixtures/release/shard-readiness-v280.json",
    "fnv1a64:2193962195ba633a",
    "frozen v280 fixture is the source baseline for route-preview-command-explain-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout