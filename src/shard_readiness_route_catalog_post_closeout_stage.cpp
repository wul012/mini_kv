#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-retention-map",
    145,
    "v345",
    "maps retained fixtures, screenshots, explanations, and walkthroughs as evidence metadata",
    "v344",
    "fixtures/release/shard-readiness-v344.json",
    "fnv1a64:a3ab2b889b3c75df",
    "frozen v344 fixture is the source baseline for route preview verification report archive-retention-map evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
