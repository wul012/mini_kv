#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-retention-audit",
    171,
    "v371",
    "audits closeout retention metadata without scanning runtime directories",
    "v370",
    "fixtures/release/shard-readiness-v370.json",
    "fnv1a64:f31e435e5f8ab4ad",
    "frozen v370 fixture is the source baseline for route preview verification report archive-closeout-retention-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout