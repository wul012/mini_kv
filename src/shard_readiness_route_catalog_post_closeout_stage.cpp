#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-fixture-freeze",
    153,
    "v353",
    "freezes archive manifest fixture semantics for historical replay",
    "v352",
    "fixtures/release/shard-readiness-v352.json",
    "fnv1a64:67bc128aadc1681f",
    "frozen v352 fixture is the source baseline for route preview verification report archive-fixture-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
