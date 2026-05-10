#include "minikv/snapshot.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

int main() {
    using namespace std::chrono_literals;

    const auto path = std::filesystem::path{"minikv-snapshot-test.snap"};
    std::filesystem::remove(path);

    minikv::Store store;
    assert(store.set("name", "mini-kv"));
    assert(store.set("phrase", "hello from snapshot"));
    assert(store.expire("name", 60s));

    std::size_t saved = 0;
    assert(minikv::SnapshotFile::save(store, path, &saved));
    assert(saved == 2);

    minikv::Store restored;
    std::size_t loaded = 0;
    assert(minikv::SnapshotFile::load(restored, path, &loaded));
    assert(loaded == 2);
    assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
    assert(restored.ttl("name").has_value());
    assert(restored.get("phrase") == std::optional<std::string>{"hello from snapshot"});

    std::filesystem::remove(path);

    const auto corrupt_path = std::filesystem::path{"minikv-snapshot-corrupt-test.snap"};
    std::filesystem::remove(corrupt_path);

    minikv::Store protected_store;
    assert(protected_store.set("existing", "survives"));

    {
        std::ofstream output{corrupt_path};
        output << "MINIKV_SNAPSHOT 1\n";
        output << "ENTRY - \"broken\"";
    }

    loaded = 123;
    assert(!minikv::SnapshotFile::load(protected_store, corrupt_path, &loaded));
    assert(loaded == 123);
    assert(protected_store.get("existing") == std::optional<std::string>{"survives"});
    assert(protected_store.size() == 1);

    std::filesystem::remove(corrupt_path);

    const auto atomic_dir = std::filesystem::path{"minikv-snapshot-atomic-test"};
    const auto atomic_path = atomic_dir / "state.snap";
    std::filesystem::remove_all(atomic_dir);
    std::filesystem::create_directories(atomic_dir);

    auto has_temp_snapshot = [&]() {
        for (const auto& entry : std::filesystem::directory_iterator(atomic_dir)) {
            if (entry.path().filename().string().find(".tmp.") != std::string::npos) {
                return true;
            }
        }
        return false;
    };

    minikv::Store first_snapshot;
    assert(first_snapshot.set("name", "old"));
    saved = 0;
    assert(minikv::SnapshotFile::save(first_snapshot, atomic_path, &saved));
    assert(saved == 1);
    assert(std::filesystem::exists(atomic_path));
    assert(!has_temp_snapshot());

    minikv::Store second_snapshot;
    assert(second_snapshot.set("name", "new"));
    assert(second_snapshot.set("extra", "value"));
    saved = 0;
    assert(minikv::SnapshotFile::save(second_snapshot, atomic_path, &saved));
    assert(saved == 2);
    assert(!has_temp_snapshot());

    minikv::Store replaced;
    loaded = 0;
    assert(minikv::SnapshotFile::load(replaced, atomic_path, &loaded));
    assert(loaded == 2);
    assert(replaced.get("name") == std::optional<std::string>{"new"});
    assert(replaced.get("extra") == std::optional<std::string>{"value"});

    std::filesystem::remove_all(atomic_dir);

    return 0;
}
