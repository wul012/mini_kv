#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-command-contract",
    161,
    "v361",
    "publishes the archive closeout command contract for the twenty-version archive chain",
    "v360",
    "fixtures/release/shard-readiness-v360.json",
    "fnv1a64:ce4589e8c5ffd352",
    "frozen v360 fixture is the source baseline for route preview verification report archive-closeout-command-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
