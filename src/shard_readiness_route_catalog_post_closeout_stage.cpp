#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-boundary-field-audit",
    93,
    "v293",
    "audits route preview read-only boundary fields",
    "v292",
    "fixtures/release/shard-readiness-v292.json",
    "fnv1a64:2193962195ba633a",
    "frozen v292 fixture is the source baseline for route-preview-boundary-field-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout