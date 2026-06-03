#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-cli-smoke-contract",
    124,
    "v324",
    "records CLI smoke expectations for read-only route preview verification report with no active router",
    "v323",
    "fixtures/release/shard-readiness-v323.json",
    "fnv1a64:2193962195ba633a",
    "frozen v323 fixture is the source baseline for route preview verification report cli-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
