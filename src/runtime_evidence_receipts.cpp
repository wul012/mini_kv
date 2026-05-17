#include "minikv/runtime_evidence_receipts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <cstdint>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;

std::string bool_part(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string field_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
}

struct SmokeFailureTaxonomyEntry {
    std::string_view id;
    std::string_view source;
    std::string_view meaning;
    std::string_view node_action;
    bool retryable;
    bool write_risk;
};

constexpr SmokeFailureTaxonomyEntry smoke_failure_taxonomy_entries[] = {
    {"closed-window",
     "operator_window",
     "The real-read adapter window is disabled, so Node should skip mini-kv reads.",
     "Keep the rehearsal skipped until an operator opens the read-only window.",
     true,
     false},
    {"connection-refused",
     "tcp_connect",
     "mini-kv is not listening or the operator has not started it for the window.",
     "Report upstream_unreachable and do not auto-start mini-kv.",
     true,
     false},
    {"timeout",
     "tcp_read",
     "mini-kv did not return the read-only response within the adapter timeout.",
     "Record timeout evidence and keep commands read-only.",
     true,
     false},
    {"invalid-json",
     "SMOKEJSON_parse",
     "The adapter did not receive parseable JSON from SMOKEJSON.",
     "Classify the result before trusting runtime fields.",
     false,
     false},
    {"read-command-failed",
     "runtime_read_command",
     "SMOKEJSON, INFOJSON, STORAGEJSON, HEALTH, or STATSJSON returned an error.",
     "Record the failing command and stop the read window.",
     false,
     false},
    {"unsafe-surface",
     "adapter_command_plan",
     "The adapter attempted LOAD, COMPACT, SETNXEX, RESTORE, or another forbidden command.",
     "Block the rehearsal and require plan review.",
     false,
     true},
    {"unexpected-write-signal",
     "runtime_smoke_diagnostics",
     "Runtime evidence reports write/admin execution or a token created by smoke.",
     "Treat the rehearsal as failed and do not publish pass evidence.",
     false,
     true},
};

struct SmokeOperatorWindowProof {
    std::string_view consumer;
    std::string_view node_action;
    bool identity_neutral_proof;
};

constexpr SmokeOperatorWindowProof smoke_operator_window_proof = {
    "Node v200 real-read window CI archive artifact manifest",
    "verify digest before importing manual window results",
    true,
};

struct RuntimeCiEvidenceHint {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view node_action;
    bool identity_neutral_proof;
    bool no_restore_proof;
    bool upload_allowed;
};

constexpr RuntimeCiEvidenceHint runtime_ci_evidence_hint = {
    "Node v201 real-read window CI artifact manifest verification",
    "c/80/",
    "verify manifest path hints before CI artifact upload dry-run",
    true,
    true,
    false,
};

struct RuntimeArtifactRetentionEvidence {
    std::string_view consumer;
    std::string_view artifact_root;
    std::string_view artifact_path_hint;
    std::string_view retention_mode;
    std::string_view node_action;
    int retention_days;
    bool release_evidence_ready;
    bool github_artifact_upload_attempted;
    bool production_window_allowed;
};

constexpr RuntimeArtifactRetentionEvidence runtime_artifact_retention_evidence = {
    "Node v203 cross-project CI artifact retention gate",
    "c/",
    "c/81/",
    "dry-run-contract-only",
    "verify retention days and artifact path before cross-project retention gate",
    30,
    true,
    false,
    false,
};

struct RuntimeBinaryProvenanceHint {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view runtime_binary_hint;
    std::string_view release_manifest_path;
    std::string_view runtime_smoke_evidence_path;
    std::string_view node_action;
    bool read_only;
    bool load_restore_compact_executed;
    bool production_binary_claimed;
};

constexpr RuntimeBinaryProvenanceHint runtime_binary_provenance_hint = {
    "Node v208 managed audit persistence boundary candidate",
    "c/95/",
    "cmake-build-v95/minikv_server and cmake-build-v95/minikv_client from the current CMake build",
    "fixtures/release/verification-manifest.json",
    "fixtures/release/runtime-smoke-evidence.json",
    "verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work",
    true,
    false,
    false,
};

struct RuntimeRetentionProvenanceCheck {
    std::string_view consumer;
    std::string_view artifact_path_hint;
    std::string_view release_manifest_path;
    std::string_view runtime_smoke_evidence_path;
    std::string_view retention_source_path_hint;
    std::string_view provenance_source;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool managed_audit_write_executed;
};

constexpr RuntimeRetentionProvenanceCheck runtime_retention_provenance_check = {
    "Node v211 managed audit identity approval provenance dry-run packet",
    "c/95/",
    "fixtures/release/verification-manifest.json",
    "fixtures/release/runtime-smoke-evidence.json",
    "c/81/",
    "binary_provenance.provenance_digest",
    "verify runtime evidence retention and binary provenance alignment before managed audit dry-run packet work",
    true,
    false,
    false,
};

struct RuntimeRetentionProvenanceReplayMarker {
    std::string_view consumer;
    std::string_view consumed_by;
    std::string_view consumed_release_version;
    std::string_view consumed_artifact_path_hint;
    std::string_view consumed_check_digest;
    std::string_view current_artifact_path_hint;
    std::string_view node_action;
    bool read_only;
    bool execution_allowed;
    bool replay_executed;
    bool managed_audit_write_executed;
};

constexpr RuntimeRetentionProvenanceReplayMarker runtime_retention_provenance_replay_marker = {
    "Node v213 managed audit packet restore drill plan",
    "Node v211 managed audit identity approval provenance dry-run packet",
    "v84",
    "c/84/",
    "fnv1a64:357cc7e9eec3f223",
    "c/95/",
    "verify v84 retention provenance consumption before managed audit packet restore drill planning",
    true,
    false,
    false,
    false,
};

std::string uptime_bucket_for_seconds(std::int64_t uptime_seconds) {
    if (uptime_seconds < 60) {
        return "lt_60s";
    }
    if (uptime_seconds < 300) {
        return "lt_5m";
    }
    if (uptime_seconds < 3600) {
        return "lt_1h";
    }
    return "gte_1h";
}

} // namespace

std::string read_command_list_digest(const std::vector<std::string>& commands) {
    std::vector<DigestPart> parts;
    parts.push_back({std::to_string(commands.size())});
    for (const auto& command : commands) {
        parts.push_back({command});
    }
    return receipt_digest("mini-kv-read-command-list", parts);
}

std::string format_live_read_session_hint_json(std::int64_t uptime_seconds,
                                               const std::vector<std::string>& read_commands) {
    return "{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\","
           "\"session_id_echo\":\"mini-kv-live-read-v95\","
           "\"server_uptime_bucket\":" + field_string(uptime_bucket_for_seconds(uptime_seconds)) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"read_command_count\":" + std::to_string(read_commands.size()) +
           ",\"read_commands\":" + string_array(read_commands) +
           ",\"write_commands_allowed\":false,\"auto_start_allowed\":false,"
           "\"node_action\":\"verify session echo, uptime bucket, and read command digest before real-read execution packet\"}";
}

std::string smoke_failure_taxonomy_digest() {
    std::vector<DigestPart> parts = {
        {"1"},
        {"Node v196 imported window result packet"},
        {std::to_string(std::size(smoke_failure_taxonomy_entries))},
    };
    for (const auto& entry : smoke_failure_taxonomy_entries) {
        parts.push_back({std::string{entry.id}});
        parts.push_back({std::string{entry.source}});
        parts.push_back({std::string{entry.meaning}});
        parts.push_back({std::string{entry.node_action}});
        parts.push_back({bool_part(entry.retryable)});
        parts.push_back({"false"});
        parts.push_back({bool_part(entry.write_risk)});
    }
    return receipt_digest("mini-kv-smoke-failure-taxonomy", parts);
}

std::string format_smoke_failure_taxonomy_json() {
    const auto taxonomy_digest = smoke_failure_taxonomy_digest();

    std::string response =
        "{\"schema_version\":1,\"consumer\":\"Node v196 imported window result packet\","
        "\"taxonomy_digest\":" + field_string(taxonomy_digest) +
        ",\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\","
        "\"source_command\":\"SMOKEJSON\",\"source_version\":" + field_string(version) +
        ","
        "\"expected_taxonomy_digest\":" + field_string(taxonomy_digest) +
        ",\"read_only\":true,\"execution_allowed\":false,\"restore_execution_allowed\":false,"
        "\"order_authoritative\":false,\"node_action\":\"verify digest before importing manual window results\"},"
        "\"categories\":[";
    for (std::size_t index = 0; index < std::size(smoke_failure_taxonomy_entries); ++index) {
        if (index != 0) {
            response += ",";
        }
        const auto& entry = smoke_failure_taxonomy_entries[index];
        response += "{\"id\":" + field_string(entry.id) +
                    ",\"source\":" + field_string(entry.source) +
                    ",\"meaning\":" + field_string(entry.meaning) +
                    ",\"node_action\":" + field_string(entry.node_action) +
                    ",\"retryable\":" + field_bool(entry.retryable) +
                    ",\"safe_to_auto_start\":false" +
                    ",\"write_risk\":" + field_bool(entry.write_risk) + "}";
    }
    response += "]}";
    return response;
}

std::string format_smoke_operator_window_proof_json() {
    return "{\"consumer\":" + field_string(smoke_operator_window_proof.consumer) +
           ",\"identity_neutral_proof\":" +
           field_bool(smoke_operator_window_proof.identity_neutral_proof) +
           ",\"node_action\":" + field_string(smoke_operator_window_proof.node_action) + "}";
}

std::string format_runtime_ci_evidence_hint_json() {
    return "{\"consumer\":" + field_string(runtime_ci_evidence_hint.consumer) +
           ",\"artifact_path_hint\":" + field_string(runtime_ci_evidence_hint.artifact_path_hint) +
           ",\"identity_neutral_proof\":" +
           field_bool(runtime_ci_evidence_hint.identity_neutral_proof) +
           ",\"no_restore_proof\":" + field_bool(runtime_ci_evidence_hint.no_restore_proof) +
           ",\"upload_allowed\":" + field_bool(runtime_ci_evidence_hint.upload_allowed) +
           ",\"node_action\":" + field_string(runtime_ci_evidence_hint.node_action) + "}";
}

std::string format_runtime_artifact_retention_evidence_json() {
    return "{\"consumer\":" + field_string(runtime_artifact_retention_evidence.consumer) +
           ",\"artifact_root\":" + field_string(runtime_artifact_retention_evidence.artifact_root) +
           ",\"artifact_path_hint\":" +
           field_string(runtime_artifact_retention_evidence.artifact_path_hint) +
           ",\"retention_days\":" +
           std::to_string(runtime_artifact_retention_evidence.retention_days) +
           ",\"retention_mode\":" +
           field_string(runtime_artifact_retention_evidence.retention_mode) +
           ",\"release_evidence_ready\":" +
           field_bool(runtime_artifact_retention_evidence.release_evidence_ready) +
           ",\"github_artifact_upload_attempted\":" +
           field_bool(runtime_artifact_retention_evidence.github_artifact_upload_attempted) +
           ",\"production_window_allowed\":" +
           field_bool(runtime_artifact_retention_evidence.production_window_allowed) +
           ",\"node_action\":" +
           field_string(runtime_artifact_retention_evidence.node_action) + "}";
}

std::string binary_provenance_digest() {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{runtime_binary_provenance_hint.artifact_path_hint}},
        {std::string{runtime_binary_provenance_hint.runtime_binary_hint}},
        {std::string{runtime_binary_provenance_hint.release_manifest_path}},
        {std::string{runtime_binary_provenance_hint.runtime_smoke_evidence_path}},
        {bool_part(runtime_binary_provenance_hint.load_restore_compact_executed)},
        {bool_part(runtime_binary_provenance_hint.production_binary_claimed)},
    };
    return receipt_digest("mini-kv-binary-provenance", parts);
}

std::string format_runtime_binary_provenance_hint_json() {
    return "{\"consumer\":" + field_string(runtime_binary_provenance_hint.consumer) +
           ",\"source_version\":" + field_string(version) +
           ",\"artifact_path_hint\":" + field_string(runtime_binary_provenance_hint.artifact_path_hint) +
           ",\"runtime_binary_hint\":" + field_string(runtime_binary_provenance_hint.runtime_binary_hint) +
           ",\"release_manifest_path\":" + field_string(runtime_binary_provenance_hint.release_manifest_path) +
           ",\"runtime_smoke_evidence_path\":" +
           field_string(runtime_binary_provenance_hint.runtime_smoke_evidence_path) +
           ",\"provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"read_only\":" + field_bool(runtime_binary_provenance_hint.read_only) +
           ",\"load_restore_compact_executed\":" +
           field_bool(runtime_binary_provenance_hint.load_restore_compact_executed) +
           ",\"production_binary_claimed\":" +
           field_bool(runtime_binary_provenance_hint.production_binary_claimed) +
           ",\"node_action\":" + field_string(runtime_binary_provenance_hint.node_action) + "}";
}

std::string retention_provenance_check_digest() {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{runtime_retention_provenance_check.artifact_path_hint}},
        {std::string{runtime_retention_provenance_check.release_manifest_path}},
        {std::string{runtime_retention_provenance_check.runtime_smoke_evidence_path}},
        {std::string{runtime_retention_provenance_check.retention_source_path_hint}},
        {binary_provenance_digest()},
        {bool_part(runtime_retention_provenance_check.read_only)},
        {bool_part(runtime_retention_provenance_check.execution_allowed)},
        {bool_part(runtime_retention_provenance_check.managed_audit_write_executed)},
    };
    return receipt_digest("mini-kv-retention-provenance-check", parts);
}

std::string format_runtime_retention_provenance_check_json() {
    return "{\"consumer\":" + field_string(runtime_retention_provenance_check.consumer) +
           ",\"source_version\":" + field_string(version) +
           ",\"artifact_path_hint\":" +
           field_string(runtime_retention_provenance_check.artifact_path_hint) +
           ",\"release_manifest_path\":" +
           field_string(runtime_retention_provenance_check.release_manifest_path) +
           ",\"runtime_smoke_evidence_path\":" +
           field_string(runtime_retention_provenance_check.runtime_smoke_evidence_path) +
           ",\"retention_source_path_hint\":" +
           field_string(runtime_retention_provenance_check.retention_source_path_hint) +
           ",\"provenance_source\":" +
           field_string(runtime_retention_provenance_check.provenance_source) +
           ",\"expected_binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"read_only\":" + field_bool(runtime_retention_provenance_check.read_only) +
           ",\"execution_allowed\":" +
           field_bool(runtime_retention_provenance_check.execution_allowed) +
           ",\"managed_audit_write_executed\":" +
           field_bool(runtime_retention_provenance_check.managed_audit_write_executed) +
           ",\"node_action\":" + field_string(runtime_retention_provenance_check.node_action) + "}";
}

std::string retention_provenance_replay_marker_digest() {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{runtime_retention_provenance_replay_marker.consumed_by}},
        {std::string{runtime_retention_provenance_replay_marker.consumed_release_version}},
        {std::string{runtime_retention_provenance_replay_marker.consumed_artifact_path_hint}},
        {std::string{runtime_retention_provenance_replay_marker.consumed_check_digest}},
        {std::string{runtime_retention_provenance_replay_marker.current_artifact_path_hint}},
        {bool_part(runtime_retention_provenance_replay_marker.read_only)},
        {bool_part(runtime_retention_provenance_replay_marker.execution_allowed)},
        {bool_part(runtime_retention_provenance_replay_marker.replay_executed)},
        {bool_part(runtime_retention_provenance_replay_marker.managed_audit_write_executed)},
    };
    return receipt_digest("mini-kv-retention-provenance-replay-marker", parts);
}

std::string format_runtime_retention_provenance_replay_marker_json() {
    return "{\"consumer\":" + field_string(runtime_retention_provenance_replay_marker.consumer) +
           ",\"source_version\":" + field_string(version) +
           ",\"consumed_by\":" + field_string(runtime_retention_provenance_replay_marker.consumed_by) +
           ",\"consumed_release_version\":" +
           field_string(runtime_retention_provenance_replay_marker.consumed_release_version) +
           ",\"consumed_artifact_path_hint\":" +
           field_string(runtime_retention_provenance_replay_marker.consumed_artifact_path_hint) +
           ",\"consumed_check_digest\":" +
           field_string(runtime_retention_provenance_replay_marker.consumed_check_digest) +
           ",\"current_artifact_path_hint\":" +
           field_string(runtime_retention_provenance_replay_marker.current_artifact_path_hint) +
           ",\"marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_only\":" + field_bool(runtime_retention_provenance_replay_marker.read_only) +
           ",\"execution_allowed\":" +
           field_bool(runtime_retention_provenance_replay_marker.execution_allowed) +
           ",\"replay_executed\":" +
           field_bool(runtime_retention_provenance_replay_marker.replay_executed) +
           ",\"managed_audit_write_executed\":" +
           field_bool(runtime_retention_provenance_replay_marker.managed_audit_write_executed) +
           ",\"node_action\":" + field_string(runtime_retention_provenance_replay_marker.node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
