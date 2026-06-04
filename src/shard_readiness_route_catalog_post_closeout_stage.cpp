#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-cli-smoke-contract",
    165,
    "v365",
    "records CLI smoke expectations for the archive closeout command",
    "v364",
    "fixtures/release/shard-readiness-v364.json",
    "fnv1a64:fcb0fb568757a6ce",
    "frozen v364 fixture is the source baseline for route preview verification report archive-closeout-cli-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout