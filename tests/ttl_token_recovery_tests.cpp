#include "minikv/command.hpp"
#include "minikv/snapshot.hpp"
#include "minikv/store.hpp"
#include "minikv/wal.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <optional>
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

long long epoch_millis(minikv::Store::TimePoint time_point) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
}

} // namespace

int main() {
    using namespace std::chrono_literals;

    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    const auto dir = std::filesystem::temp_directory_path() /
                     ("minikv-ttl-token-recovery-test-" + std::to_string(suffix));
    const auto snapshot_path = dir / "tokens.snap";
    const auto wal_path = dir / "tokens.wal";
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);

    {
        minikv::Store live;
        minikv::WriteAheadLog wal{wal_path};
        minikv::CommandProcessor processor{live, &wal};

        auto result = processor.execute("SETNXEX token:snapshot 60 from-snapshot");
        assert(result.response == "1");

        std::size_t saved = 0;
        assert(minikv::SnapshotFile::save(live, snapshot_path, &saved));
        assert(saved == 1);

        result = processor.execute("SETNXEX token:wal 60 from-wal");
        assert(result.response == "1");

        result = processor.execute("SETNXEX token:snapshot 60 duplicate");
        assert(result.response == "0");
        assert(live.get("token:snapshot") == std::optional<std::string>{"from-snapshot"});
        assert(live.get("token:wal") == std::optional<std::string>{"from-wal"});
    }

    {
        minikv::Store restored;
        std::size_t loaded = 0;
        assert(minikv::SnapshotFile::load(restored, snapshot_path, &loaded));
        assert(loaded == 1);
        assert(restored.get("token:snapshot") == std::optional<std::string>{"from-snapshot"});
        assert(restored.ttl("token:snapshot").has_value());

        minikv::WriteAheadLog wal{wal_path};
        const auto report = wal.replay_with_report(restored);
        assert(report.applied_records == 2);
        assert(report.skipped_records == 0);
        assert(report.truncated_records == 0);
        assert(report.checksum_failed_records == 0);
        assert(restored.get("token:snapshot") == std::optional<std::string>{"from-snapshot"});
        assert(restored.ttl("token:snapshot").has_value());
        assert(restored.get("token:wal") == std::optional<std::string>{"from-wal"});
        assert(restored.ttl("token:wal").has_value());
    }

    const auto expired_wal_path = dir / "expired-token.wal";
    {
        minikv::WriteAheadLog wal{expired_wal_path};
        const auto expired_at = epoch_millis(minikv::Store::Clock::now() - 1s);
        assert(wal.append("SETEXAT token:expired " + std::to_string(expired_at) + " old"));

        minikv::Store restored;
        const auto report = wal.replay_with_report(restored);
        assert(report.applied_records == 1);
        assert(report.skipped_records == 0);
        assert(!restored.get("token:expired").has_value());
        assert(restored.size() == 0);
    }

    const auto expired_snapshot_path = dir / "expired-token.snap";
    {
        const auto expired_at = epoch_millis(minikv::Store::Clock::now() - 1s);
        std::ofstream output{expired_snapshot_path};
        output << "MINIKV_SNAPSHOT 1\n";
        output << "ENTRY " << expired_at << " \"token:snapshot-expired\" \"old\"\n";
    }

    {
        minikv::Store restored;
        std::size_t loaded = 0;
        assert(minikv::SnapshotFile::load(restored, expired_snapshot_path, &loaded));
        assert(loaded == 1);
        assert(!restored.get("token:snapshot-expired").has_value());
        assert(restored.size() == 0);
    }

    const auto compact_path = dir / "compacted-token.wal";
    {
        minikv::Store live;
        minikv::WriteAheadLog wal{compact_path};
        minikv::CommandProcessor processor{live, &wal};

        auto result = processor.execute("SETNXEX token:compact 60 compacted");
        assert(result.response == "1");

        result = processor.execute("COMPACT");
        assert(result.response == "OK compacted 1");
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{compact_path};
        const auto report = wal.replay_with_report(restored);
        assert(report.applied_records == 2);
        assert(report.skipped_records == 0);
        assert(restored.get("token:compact") == std::optional<std::string>{"compacted"});
        assert(restored.ttl("token:compact").has_value());
    }

    const auto recovery_fixture = read_fixture_text(std::filesystem::path{"fixtures"} / "ttl-token" /
                                                    "recovery-evidence.json");
    assert_contains(recovery_fixture, "\"evidence_version\":\"mini-kv-ttl-token-recovery.v1\"");
    assert_contains(recovery_fixture, "\"capability\":\"ttl_token_recovery\"");
    assert_contains(recovery_fixture, "\"order_authoritative\":false");
    assert_contains(recovery_fixture, "\"claim_record\":\"SETEXAT key epoch_millis value\"");
    assert_contains(recovery_fixture, "\"replay_restores_unexpired_token\":true");
    assert_contains(recovery_fixture, "\"replay_drops_expired_token\":true");
    assert_contains(recovery_fixture, "\"snapshot_load_drops_expired_token\":true");
    assert_contains(recovery_fixture, "\"restart_merges_snapshot_and_wal_tokens\":true");
    assert_contains(recovery_fixture, "not connected to Java transaction chain");
    assert_contains(recovery_fixture, "Node v161 controlled idempotency drill runbook");

    const auto index_fixture = read_fixture_text(std::filesystem::path{"fixtures"} / "ttl-token" / "index.json");
    assert_contains(index_fixture, "\"recovery_evidence_path\":\"fixtures/ttl-token/recovery-evidence.json\"");
    assert_contains(index_fixture, "\"replay_drops_expired_token\":true");
    assert_contains(index_fixture, "\"snapshot_load_drops_expired_token\":true");
    assert_contains(index_fixture, "\"restart_merges_snapshot_and_wal_tokens\":true");

    std::filesystem::remove_all(dir);
    return 0;
}
