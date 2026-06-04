#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-java-echo-audit",
    152,
    "v352",
    "audits Java echo expectations without granting mini-kv authority",
    "v351",
    "fixtures/release/shard-readiness-v351.json",
    "fnv1a64:85099e82c1bad69c",
    "frozen v351 fixture is the source baseline for route preview verification report archive-java-echo-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
