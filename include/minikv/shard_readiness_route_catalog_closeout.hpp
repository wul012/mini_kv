#pragma once

#include <string>

namespace minikv::shard_readiness::route_catalog_closeout {

std::string format_closeout_snapshot_json();
std::string format_anchor_removal_audit_json();
std::string format_expected_integrity_snapshot_audit_json();
std::string format_expected_integrity_snapshot_freeze_json();

} // namespace minikv::shard_readiness::route_catalog_closeout
