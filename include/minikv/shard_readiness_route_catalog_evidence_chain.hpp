#pragma once

#include <string>

namespace minikv::shard_readiness::route_catalog_evidence_chain {

std::string format_route_catalog_evidence_chain_json();
std::string format_route_catalog_evidence_chain_audit_json();
std::string format_route_catalog_cleanup_closeout_handoff_json();
std::string format_route_catalog_cleanup_closeout_handoff_audit_json();
std::string format_route_catalog_cleanup_closeout_handoff_audit_freeze_json();
std::string format_route_catalog_cleanup_closeout_release_catalog_json();
std::string format_route_catalog_cleanup_closeout_release_catalog_audit_json();

} // namespace minikv::shard_readiness::route_catalog_evidence_chain
