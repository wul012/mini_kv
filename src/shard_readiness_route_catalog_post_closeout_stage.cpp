#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-history-fallback-freeze",
    130,
    "v330",
    "freezes route preview verification report historical fallback source with no active router",
    "v329",
    "fixtures/release/shard-readiness-v329.json",
    "fnv1a64:2193962195ba633a",
    "frozen v329 fixture is the source baseline for route preview verification report history-fallback-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
