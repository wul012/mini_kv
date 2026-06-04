#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-retention-audit",
    146,
    "v346",
    "audits retention metadata counts without scanning runtime directories",
    "v345",
    "fixtures/release/shard-readiness-v345.json",
    "fnv1a64:68111a874e1be61a",
    "frozen v345 fixture is the source baseline for route preview verification report archive-retention-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
