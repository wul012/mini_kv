#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-consumer-handoff-audit",
    173,
    "v373",
    "audits consumer handoff boundaries and no live-read requirements",
    "v372",
    "fixtures/release/shard-readiness-v372.json",
    "fnv1a64:69cd33783dfffba4",
    "frozen v372 fixture is the source baseline for route preview verification report archive-closeout-consumer-handoff-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout