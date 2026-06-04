#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-consumer-handoff-audit",
    150,
    "v350",
    "audits consumer handoff boundaries and no live-read requirements",
    "v349",
    "fixtures/release/shard-readiness-v349.json",
    "fnv1a64:c513172d8269d288",
    "frozen v349 fixture is the source baseline for route preview verification report archive-consumer-handoff-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
