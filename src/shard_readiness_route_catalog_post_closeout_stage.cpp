#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-no-execution-audit",
    96,
    "v296",
    "audits that route preview still permits no execution",
    "v295",
    "fixtures/release/shard-readiness-v295.json",
    "fnv1a64:2193962195ba633a",
    "frozen v295 fixture is the source baseline for route-preview-no-execution-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout