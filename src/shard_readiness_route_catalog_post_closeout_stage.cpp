#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-shard-readiness-summary-notes-split",
    78,
    "v278",
    "moves shard readiness summary notes JSON construction into a dedicated source file",
    "v277",
    "fixtures/release/shard-readiness-v277.json",
    "fnv1a64:ce4d290795868b84",
    "frozen v277 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
