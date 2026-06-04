#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-verification",
    142,
    "v342",
    "verifies the manifest fields without reading archive files or enabling execution",
    "v341",
    "fixtures/release/shard-readiness-v341.json",
    "fnv1a64:934caa72499a8f77",
    "frozen v341 fixture is the source baseline for route preview verification report archive-verification evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
