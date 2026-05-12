#include "minikv/recovery_evidence.hpp"
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

} // namespace

int main() {
    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    const auto dir = std::filesystem::temp_directory_path() /
                     ("minikv-recovery-evidence-test-" + std::to_string(suffix));
    const auto snapshot_path = dir / "baseline.snap";
    const auto wal_path = dir / "restart.wal";
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);

    minikv::Store before_restart;
    assert(before_restart.set("recovery:base", "snapshot-value"));

    std::size_t saved = 0;
    assert(minikv::SnapshotFile::save(before_restart, snapshot_path, &saved));
    assert(saved == 1);

    {
        minikv::WriteAheadLog wal{wal_path};
        assert(wal.append("SET recovery:delta wal-value"));
    }
    assert(before_restart.set("recovery:delta", "wal-value"));

    minikv::Store after_restart;
    std::size_t loaded = 0;
    assert(minikv::SnapshotFile::load(after_restart, snapshot_path, &loaded));
    assert(loaded == 1);

    minikv::WriteAheadLog replay_wal{wal_path};
    const auto replay = replay_wal.replay_with_report(after_restart);
    assert(replay.applied_records == 1);
    assert(replay.skipped_records == 0);
    assert(replay.truncated_records == 0);
    assert(replay.checksum_failed_records == 0);
    assert(after_restart.get("recovery:base") == std::optional<std::string>{"snapshot-value"});
    assert(after_restart.get("recovery:delta") == std::optional<std::string>{"wal-value"});

    const auto evidence = minikv::make_restart_recovery_evidence(before_restart, after_restart, saved, replay);
    assert(evidence.recovered);
    assert(evidence.digests_match);
    assert(evidence.before.key_count == 2);
    assert(evidence.after.key_count == 2);
    assert(evidence.before.digest == "fnv1a64:0991cf4e41a8005a");
    assert(evidence.after.digest == evidence.before.digest);

    const auto json = minikv::format_restart_recovery_evidence_json(evidence);
    const auto fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json");
    assert(json == fixture);

    std::filesystem::remove_all(dir);
    return 0;
}
