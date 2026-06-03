#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-java-echo-handoff",
    92,
    "v292",
    "documents Java echo handoff for route preview evidence",
    "v291",
    "fixtures/release/shard-readiness-v291.json",
    "fnv1a64:2193962195ba633a",
    "frozen v291 fixture is the source baseline for route-preview-java-echo-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout