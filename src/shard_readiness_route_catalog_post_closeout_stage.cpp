#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-command-contract",
    120,
    "v320",
    "adds read-only SHARDROUTEVERIFYREPORTJSON preview verification report command with no active router",
    "v319",
    "fixtures/release/shard-readiness-v319.json",
    "fnv1a64:6c69270142ff05f0",
    "frozen v319 fixture is the source baseline for route preview verification report command-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
