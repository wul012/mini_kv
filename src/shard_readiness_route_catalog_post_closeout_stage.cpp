#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-tcp-smoke-contract",
    166,
    "v366",
    "records TCP smoke expectations for the archive closeout command",
    "v365",
    "fixtures/release/shard-readiness-v365.json",
    "fnv1a64:31a147edd9e5183c",
    "frozen v365 fixture is the source baseline for route preview verification report archive-closeout-tcp-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout