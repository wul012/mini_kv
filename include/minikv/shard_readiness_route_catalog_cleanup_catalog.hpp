#pragma once

#include <string>

namespace minikv::shard_readiness::route_catalog_cleanup_catalog {

std::string format_route_catalog_cleanup_closeout_release_catalog_json();
std::string format_route_catalog_cleanup_closeout_release_catalog_audit_json();
std::string format_route_catalog_cleanup_closeout_catalog_maintenance_json();
std::string format_latest_route_catalog_cleanup_evidence_package_json();
std::string format_latest_route_catalog_cleanup_evidence_package_audit_json();

} // namespace minikv::shard_readiness::route_catalog_cleanup_catalog
