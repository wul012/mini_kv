#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_contains(const std::string& text, std::string_view expected) {
    assert(text.find(std::string{expected}) != std::string::npos);
}

std::size_t count_occurrences(const std::string& text, std::string_view needle) {
    std::size_t count = 0;
    std::size_t position = 0;
    while ((position = text.find(needle, position)) != std::string::npos) {
        ++count;
        position += needle.size();
    }
    return count;
}

} // namespace

int main() {
    const auto index_path = std::filesystem::path{"fixtures"} / "recovery" / "index.json";
    const auto evidence_path = std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json";
    const auto index = read_fixture_text(index_path);
    const auto evidence = read_fixture_text(evidence_path);

    assert_contains(index, "\"index_version\":\"mini-kv-recovery-fixtures.v1\"");
    assert_contains(index, "\"read_only\":true");
    assert_contains(index, "\"execution_allowed\":false");
    assert_contains(index, "\"order_authoritative\":false");
    assert_contains(index, "\"fixture_count\":1");
    assert_contains(index, "\"path\":\"fixtures/recovery/restart-recovery-evidence.json\"");
    assert(count_occurrences(index, "\"id\":\"restart-recovery-evidence\"") == 1);

    assert_contains(index, "\"evidence_version\":\"mini-kv-restart-recovery.v1\"");
    assert_contains(index, "\"recovered\":true");
    assert_contains(index, "\"digests_match\":true");
    assert_contains(index, "\"digest\":\"fnv1a64:0991cf4e41a8005a\"");
    assert_contains(index, "\"baseline_saved_keys\":1");
    assert_contains(index, "\"replay_records\":1");
    assert_contains(index, "\"replay_work\":\"one_delta_record_after_snapshot_baseline\"");
    assert_contains(index, "\"restart_replay_cost\":{\"cost_model\":\"sample_record_count\","
                           "\"snapshot_loads\":1,\"wal_records_replayed\":1");
    assert_contains(index, "\"expected_replay_complexity\":\"O(snapshot_keys + wal_records)\"");
    assert_contains(index, "\"measured_in_fixture_only\":true");
    assert_contains(index, "\"retention_boundary\":{\"fixture_sample_only\":true,"
                           "\"values_retained_in_index\":false");
    assert_contains(index, "\"production_retention_policy\":\"not_defined\"");
    assert_contains(index, "\"backup_or_rotation_policy\":\"not_included\"");
    assert_contains(index, "\"operator_action_required\":\"define retention before production use\"");
    assert_contains(index, "\"consumer_hint\":\"Node v107 production readiness summary v4\"");
    assert_contains(index, "\"boundaries\":[\"read-only fixture\",\"no server command added\","
                           "\"no write command expanded\",\"not order authoritative\","
                           "\"retention policy not defined\"]");
    assert_contains(index, "\"restart replay cost is sample-scale evidence only\"");
    assert_contains(index, "\"retention boundary must be defined outside this fixture\"");

    assert_contains(evidence, "\"evidence_version\":\"mini-kv-restart-recovery.v1\"");
    assert_contains(evidence, "\"recovered\":true");
    assert_contains(evidence, "\"digests_match\":true");
    assert_contains(evidence, "\"digest\":\"fnv1a64:0991cf4e41a8005a\"");
    assert_contains(evidence, "\"order_authoritative\":false");
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / evidence_path));

    return 0;
}
