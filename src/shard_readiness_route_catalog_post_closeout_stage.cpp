#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-no-execution-audit",
    158,
    "v358",
    "audits that archive evidence permits no execution",
    "v357",
    "fixtures/release/shard-readiness-v357.json",
    "fnv1a64:7cf53729ea0bcd67",
    "frozen v357 fixture is the source baseline for route preview verification report archive-no-execution-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
