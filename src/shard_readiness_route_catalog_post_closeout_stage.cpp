#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-command-catalog-alignment",
    80,
    "v280",
    "aligns route preview commands with command catalog and help surfaces",
    "v279",
    "fixtures/release/shard-readiness-v279.json",
    "fnv1a64:2193962195ba633a",
    "frozen v279 fixture is the source baseline for route-preview-command-catalog-alignment evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout