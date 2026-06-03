#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-no-router-audit",
    135,
    "v335",
    "audits that route preview verification report still installs no active router with no active router",
    "v334",
    "fixtures/release/shard-readiness-v334.json",
    "fnv1a64:2193962195ba633a",
    "frozen v334 fixture is the source baseline for route preview verification report no-router-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
