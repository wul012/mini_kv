#include "minikv/command.hpp"
#include "minikv/store.hpp"

#include <cassert>
#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <vector>

int main() {
    using namespace std::chrono_literals;

    constexpr std::size_t thread_count = 4;
    constexpr std::size_t operations_per_thread = 1000;

    minikv::Store store;
    std::vector<std::thread> writers;
    writers.reserve(thread_count);

    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
        writers.emplace_back([&store, thread_index] {
            minikv::CommandProcessor processor{store};

            for (std::size_t i = 0; i < operations_per_thread; ++i) {
                const auto key = "stress:" + std::to_string(thread_index) + ":" + std::to_string(i);
                const auto value = "value:" + std::to_string(i);

                auto result = processor.execute("SET " + key + " " + value);
                assert(result.response == "OK inserted");

                result = processor.execute("GET " + key);
                assert(result.response == value);

                if (i % 3 == 0) {
                    result = processor.execute("EXPIRE " + key + " 60");
                    assert(result.response == "1");

                    result = processor.execute("TTL " + key);
                    const auto ttl = std::stoll(result.response);
                    assert(ttl > 0);
                    assert(ttl <= 60);
                }
            }
        });
    }

    for (auto& writer : writers) {
        writer.join();
    }

    const auto total_keys = thread_count * operations_per_thread;
    assert(store.size() == total_keys);

    const auto items = store.snapshot_items();
    assert(items.size() == total_keys);

    minikv::Store restored;
    restored.restore_snapshot(items);
    assert(restored.size() == total_keys);

    std::vector<std::thread> erasers;
    erasers.reserve(thread_count);

    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
        erasers.emplace_back([&store, thread_index] {
            for (std::size_t i = 0; i < operations_per_thread; i += 2) {
                const auto key = "stress:" + std::to_string(thread_index) + ":" + std::to_string(i);
                assert(store.erase(key));
            }
        });
    }

    for (auto& eraser : erasers) {
        eraser.join();
    }

    assert(store.size() == total_keys / 2);

    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
        const auto remaining_key = "stress:" + std::to_string(thread_index) + ":1";
        const auto erased_key = "stress:" + std::to_string(thread_index) + ":0";

        assert(store.get(remaining_key) == std::optional<std::string>{"value:1"});
        assert(!store.get(erased_key).has_value());
    }

    std::this_thread::sleep_for(1ms);
    assert(store.size() == total_keys / 2);

    return 0;
}
