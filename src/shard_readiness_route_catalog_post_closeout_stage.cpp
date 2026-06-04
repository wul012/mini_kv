#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-tcp-smoke-contract",
    155,
    "v355",
    "records TCP smoke expectations for the archive command",
    "v354",
    "fixtures/release/shard-readiness-v354.json",
    "fnv1a64:f525662d1524cae5",
    "frozen v354 fixture is the source baseline for route preview verification report archive-tcp-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
