#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-java-echo-handoff",
    174,
    "v374",
    "records Java echo handoff semantics for archive closeout fields",
    "v373",
    "fixtures/release/shard-readiness-v373.json",
    "fnv1a64:c5ad15aff90c27c9",
    "frozen v373 fixture is the source baseline for route preview verification report archive-closeout-java-echo-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout