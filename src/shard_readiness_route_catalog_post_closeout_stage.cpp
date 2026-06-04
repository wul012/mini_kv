#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-no-router-audit",
    156,
    "v356",
    "audits that archive evidence installs no active router",
    "v355",
    "fixtures/release/shard-readiness-v355.json",
    "fnv1a64:34814735ed068cec",
    "frozen v355 fixture is the source baseline for route preview verification report archive-no-router-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
