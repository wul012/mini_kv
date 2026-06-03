#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-no-write-audit",
    95,
    "v295",
    "audits that route preview still performs no writes",
    "v294",
    "fixtures/release/shard-readiness-v294.json",
    "fnv1a64:2193962195ba633a",
    "frozen v294 fixture is the source baseline for route-preview-no-write-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout