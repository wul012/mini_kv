#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-fixture-freeze",
    167,
    "v367",
    "freezes archive closeout fixture semantics for historical replay",
    "v366",
    "fixtures/release/shard-readiness-v366.json",
    "fnv1a64:7d30ce173c4909ff",
    "frozen v366 fixture is the source baseline for route preview verification report archive-closeout-fixture-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout