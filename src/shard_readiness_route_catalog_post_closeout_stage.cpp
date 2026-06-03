#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-sample-parity-audit",
    127,
    "v327",
    "audits report samples against route preview verification samples with no active router",
    "v326",
    "fixtures/release/shard-readiness-v326.json",
    "fnv1a64:2193962195ba633a",
    "frozen v326 fixture is the source baseline for route preview verification report sample-parity-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
