#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-checkjson-contract",
    123,
    "v323",
    "freezes CHECKJSON route preview verification report no-execution contract with no active router",
    "v322",
    "fixtures/release/shard-readiness-v322.json",
    "fnv1a64:2193962195ba633a",
    "frozen v322 fixture is the source baseline for route preview verification report checkjson-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
