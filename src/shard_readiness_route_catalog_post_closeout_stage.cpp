#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-explain-contract",
    122,
    "v322",
    "freezes EXPLAINJSON route preview verification report semantics as metadata read only with no active router",
    "v321",
    "fixtures/release/shard-readiness-v321.json",
    "fnv1a64:2193962195ba633a",
    "frozen v321 fixture is the source baseline for route preview verification report explain-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
