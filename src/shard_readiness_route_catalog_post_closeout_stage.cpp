#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-command-contract",
    79,
    "v279",
    "adds read-only SHARDROUTE and SHARDROUTEJSON preview commands with no active router",
    "v278",
    "fixtures/release/shard-readiness-v278.json",
    "fnv1a64:26f55eb7b8108367",
    "frozen v278 fixture is the source baseline for route preview command contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
