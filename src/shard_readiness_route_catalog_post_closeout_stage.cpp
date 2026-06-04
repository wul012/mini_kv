#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-boundary-field-audit",
    176,
    "v376",
    "audits boundary fields for read-only archive closeout consumption",
    "v375",
    "fixtures/release/shard-readiness-v375.json",
    "fnv1a64:7e37f53d1d19c6a8",
    "frozen v375 fixture is the source baseline for route preview verification report archive-closeout-boundary-field-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout