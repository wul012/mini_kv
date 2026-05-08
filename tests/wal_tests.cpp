#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/wal.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <thread>

int main() {
    using namespace std::chrono_literals;

    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    const auto path = std::filesystem::temp_directory_path() / ("minikv-wal-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(path);

    {
        minikv::Store store;
        minikv::WriteAheadLog wal{path};
        minikv::CommandProcessor processor{store, &wal};

        auto result = processor.execute("SET name mini-kv");
        assert(result.response == "OK inserted");

        result = processor.execute("SET phrase hello from wal");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE name 60");
        assert(result.response == "1");

        result = processor.execute("DEL phrase");
        assert(result.response == "1");

        result = processor.execute("SET stale old");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE stale 1");
        assert(result.response == "1");

        std::this_thread::sleep_for(1100ms);

        result = processor.execute("GET stale");
        assert(result.response == "(nil)");
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{path};
        const auto applied = wal.replay(restored);

        assert(applied == 6);
        assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
        assert(restored.ttl("name").has_value());
        assert(!restored.get("phrase").has_value());
        assert(!restored.get("stale").has_value());
    }

    std::filesystem::remove(path);

    return 0;
}
