#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-history-fallback-freeze",
    89,
    "v289",
    "freezes route preview historical fallback source",
    "v288",
    "fixtures/release/shard-readiness-v288.json",
    "fnv1a64:2193962195ba633a",
    "frozen v288 fixture is the source baseline for route-preview-history-fallback-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout