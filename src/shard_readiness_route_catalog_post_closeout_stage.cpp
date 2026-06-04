#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-manifest",
    141,
    "v341",
    "publishes route preview verification report archive manifest with no active router",
    "v340",
    "fixtures/release/shard-readiness-v340.json",
    "fnv1a64:43e9e09af4d02e62",
    "frozen v340 fixture is the source baseline for route preview verification report archive manifest evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
