#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-command-catalog-alignment",
    121,
    "v321",
    "aligns route preview verification report with command catalog and help surfaces with no active router",
    "v320",
    "fixtures/release/shard-readiness-v320.json",
    "fnv1a64:2193962195ba633a",
    "frozen v320 fixture is the source baseline for route preview verification report command-catalog-alignment evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
