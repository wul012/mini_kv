#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-java-echo-handoff",
    133,
    "v333",
    "documents Java echo handoff for route preview verification report evidence with no active router",
    "v332",
    "fixtures/release/shard-readiness-v332.json",
    "fnv1a64:2193962195ba633a",
    "frozen v332 fixture is the source baseline for route preview verification report java-echo-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
