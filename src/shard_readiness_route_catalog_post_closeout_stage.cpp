#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-consumer-handoff",
    149,
    "v349",
    "records Node consumer handoff semantics for the archive manifest",
    "v348",
    "fixtures/release/shard-readiness-v348.json",
    "fnv1a64:2c9eac6c48d4776d",
    "frozen v348 fixture is the source baseline for route preview verification report archive-consumer-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
