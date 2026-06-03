#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-boundary-field-audit",
    134,
    "v334",
    "audits route preview verification report read-only boundary fields with no active router",
    "v333",
    "fixtures/release/shard-readiness-v333.json",
    "fnv1a64:2193962195ba633a",
    "frozen v333 fixture is the source baseline for route preview verification report boundary-field-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
