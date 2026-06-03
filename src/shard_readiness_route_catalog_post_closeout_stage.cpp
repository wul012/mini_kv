#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-no-execution-audit",
    137,
    "v337",
    "audits that route preview verification report still permits no execution with no active router",
    "v336",
    "fixtures/release/shard-readiness-v336.json",
    "fnv1a64:2193962195ba633a",
    "frozen v336 fixture is the source baseline for route preview verification report no-execution-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
