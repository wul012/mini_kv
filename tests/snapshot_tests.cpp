#include "minikv/snapshot.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
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

    return 0;
}
