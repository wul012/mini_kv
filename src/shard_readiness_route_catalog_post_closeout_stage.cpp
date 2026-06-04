#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-no-execution-audit",
    179,
    "v379",
    "audits that archive closeout evidence permits no execution",
    "v378",
    "fixtures/release/shard-readiness-v378.json",
    "fnv1a64:ce475d7c9ffb11ca",
    "frozen v378 fixture is the source baseline for route preview verification report archive-closeout-no-execution-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout