#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-java-echo-audit",
    175,
    "v375",
    "audits Java echo expectations without granting mini-kv authority",
    "v374",
    "fixtures/release/shard-readiness-v374.json",
    "fnv1a64:0da600fb455f2af2",
    "frozen v374 fixture is the source baseline for route preview verification report archive-closeout-java-echo-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout