#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-no-write-audit",
    157,
    "v357",
    "audits that archive evidence permits no writes",
    "v356",
    "fixtures/release/shard-readiness-v356.json",
    "fnv1a64:f455307106f61dde",
    "frozen v356 fixture is the source baseline for route preview verification report archive-no-write-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
