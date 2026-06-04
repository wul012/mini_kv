#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-java-echo-handoff",
    151,
    "v351",
    "records Java echo handoff semantics for archive fields",
    "v350",
    "fixtures/release/shard-readiness-v350.json",
    "fnv1a64:3c51622ccdb38a8f",
    "frozen v350 fixture is the source baseline for route preview verification report archive-java-echo-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
