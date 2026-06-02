#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-stability-evidence-report-readiness-audit",
    36,
    "v236",
    "audits the Node v522 stability evidence report readiness path from frozen v235 evidence",
    "v235",
    "fixtures/release/shard-readiness-v235.json",
    "fnv1a64:801952cee74210ef",
    "frozen v235 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
