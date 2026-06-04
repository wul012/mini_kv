#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-cli-smoke-contract",
    154,
    "v354",
    "records CLI smoke expectations for the archive command",
    "v353",
    "fixtures/release/shard-readiness-v353.json",
    "fnv1a64:44ee802faccd7e83",
    "frozen v353 fixture is the source baseline for route preview verification report archive-cli-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
