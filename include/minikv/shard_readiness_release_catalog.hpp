#pragma once

#include <string>

namespace minikv::shard_readiness::release_catalog {

std::string format_release_catalog_json();
std::string format_release_catalog_audit_json();

} // namespace minikv::shard_readiness::release_catalog
