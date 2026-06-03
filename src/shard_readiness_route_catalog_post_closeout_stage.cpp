#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-tcp-smoke-contract",
    125,
    "v325",
    "records TCP smoke expectations for read-only route preview verification report with no active router",
    "v324",
    "fixtures/release/shard-readiness-v324.json",
    "fnv1a64:2193962195ba633a",
    "frozen v324 fixture is the source baseline for route preview verification report tcp-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
