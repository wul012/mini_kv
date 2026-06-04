#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-no-write-audit",
    178,
    "v378",
    "audits that archive closeout evidence permits no writes",
    "v377",
    "fixtures/release/shard-readiness-v377.json",
    "fnv1a64:d0e5eaecd7466a6d",
    "frozen v377 fixture is the source baseline for route preview verification report archive-closeout-no-write-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout