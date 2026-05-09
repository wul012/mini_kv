#include "minikv/command.hpp"
#include "minikv/store.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

struct BenchResult {
    std::string name;
    std::size_t operations = 0;
    std::chrono::duration<double> elapsed{};
};

void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [operations] [key_count]\n";
}

bool parse_positive_size(std::string_view text, std::size_t& value) {
    std::string input{text};
    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(input.c_str(), &end, 10);

    if (end == input.c_str() || *end != '\0' || parsed == 0) {
        return false;
    }

    value = static_cast<std::size_t>(parsed);
    return true;
}

template <typename Fn>
BenchResult measure(std::string name, std::size_t operations, Fn&& fn) {
    const auto started = Clock::now();
    fn();
    const auto ended = Clock::now();
    return BenchResult{std::move(name), operations, ended - started};
}

void print_result(const BenchResult& result) {
    const auto seconds = result.elapsed.count();
    const auto throughput = seconds > 0.0 ? static_cast<double>(result.operations) / seconds : 0.0;

    std::cout << std::left << std::setw(18) << result.name << std::right << std::setw(12) << result.operations
              << " ops  " << std::fixed << std::setprecision(3) << std::setw(10) << seconds * 1000.0
              << " ms  " << std::setw(14) << std::setprecision(0) << throughput << " ops/sec\n";
}

std::string make_key(std::size_t index) {
    return "key:" + std::to_string(index);
}

std::string make_value(std::size_t index) {
    return "value:" + std::to_string(index);
}

std::vector<std::string> make_keys(std::size_t key_count) {
    std::vector<std::string> keys;
    keys.reserve(key_count);

    for (std::size_t i = 0; i < key_count; ++i) {
        keys.push_back(make_key(i));
    }

    return keys;
}

} // namespace

int main(int argc, char* argv[]) {
    std::size_t operations = 100000;
    std::size_t key_count = 10000;

    if (argc > 3) {
        print_usage(argv[0]);
        return 2;
    }

    if (argc >= 2 && !parse_positive_size(argv[1], operations)) {
        print_usage(argv[0]);
        return 2;
    }

    if (argc >= 3 && !parse_positive_size(argv[2], key_count)) {
        print_usage(argv[0]);
        return 2;
    }

    if (key_count > operations) {
        key_count = operations;
    }

    const auto keys = make_keys(key_count);
    minikv::Store store;
    minikv::CommandProcessor processor{store};

    std::cout << "mini-kv benchmark\n";
    std::cout << "operations: " << operations << '\n';
    std::cout << "key_count:  " << key_count << "\n\n";

    const auto store_set = measure("store.set", operations, [&] {
        for (std::size_t i = 0; i < operations; ++i) {
            store.set(keys[i % key_count], make_value(i));
        }
    });

    std::size_t hits = 0;
    const auto store_get = measure("store.get", operations, [&] {
        for (std::size_t i = 0; i < operations; ++i) {
            if (store.get(keys[i % key_count]).has_value()) {
                ++hits;
            }
        }
    });

    const auto command_set = measure("command.set", operations, [&] {
        for (std::size_t i = 0; i < operations; ++i) {
            const auto result = processor.execute("SET " + keys[i % key_count] + " " + make_value(i));
            if (result.response.empty()) {
                std::cerr << "unexpected empty SET response\n";
                std::exit(1);
            }
        }
    });

    std::size_t command_hits = 0;
    const auto command_get = measure("command.get", operations, [&] {
        for (std::size_t i = 0; i < operations; ++i) {
            const auto result = processor.execute("GET " + keys[i % key_count]);
            if (result.response != "(nil)") {
                ++command_hits;
            }
        }
    });

    const auto store_delete = measure("store.erase", key_count, [&] {
        for (const auto& key : keys) {
            store.erase(key);
        }
    });

    print_result(store_set);
    print_result(store_get);
    print_result(command_set);
    print_result(command_get);
    print_result(store_delete);

    std::cout << "\nstore.get hits:    " << hits << '\n';
    std::cout << "command.get hits:  " << command_hits << '\n';
    std::cout << "final store size:  " << store.size() << '\n';

    return 0;
}
