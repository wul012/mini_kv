#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-no-write-audit",
    136,
    "v336",
    "audits that route preview verification report still performs no writes with no active router",
    "v335",
    "fixtures/release/shard-readiness-v335.json",
    "fnv1a64:2193962195ba633a",
    "frozen v335 fixture is the source baseline for route preview verification report no-write-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
