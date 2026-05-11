#include "minikv/store.hpp"

#include <cassert>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;

    assert(store.size() == 0);
    assert(!store.contains("name"));
    assert(!store.get("name").has_value());

    assert(store.set("name", "mini-kv"));
    assert(store.size() == 1);
    assert(store.contains("name"));
    assert(store.get("name") == std::optional<std::string>{"mini-kv"});

    assert(!store.set("name", "redis-ish"));
    assert(store.size() == 1);
    assert(store.get("name") == std::optional<std::string>{"redis-ish"});

    assert(store.set("lang", "cpp20"));
    assert(store.set("alpha", "first"));
    const std::vector<std::string> expected_keys{"alpha", "lang", "name"};
    assert(store.keys() == expected_keys);

    assert(store.expire("alpha", 1s));
    std::this_thread::sleep_for(1100ms);
    const std::vector<std::string> expected_live_keys{"lang", "name"};
    assert(store.keys() == expected_live_keys);

    const auto snapshot = store.snapshot();
    assert(snapshot.size() == 2);
    assert(snapshot[0].first == "lang");
    assert(snapshot[1].first == "name");

    assert(store.erase("name"));
    assert(!store.erase("name"));
    assert(store.size() == 1);

    store.clear();
    assert(store.size() == 0);

    assert(!store.set("", "bad"));
    assert(store.size() == 0);

    assert(!store.expire("missing", 1s));
    assert(!store.ttl("missing").has_value());

    assert(store.set("reset", "v1"));
    assert(store.expire("reset", 1s));
    assert(!store.set("reset", "v2"));
    assert(store.get("reset") == std::optional<std::string>{"v2"});
    assert(store.ttl("reset") == std::optional<std::chrono::seconds>{-1s});

    store.clear();

    assert(store.set("session", "open"));
    assert(store.ttl("session") == std::optional<std::chrono::seconds>{-1s});
    assert(store.expire("session", 1s));

    const auto ttl = store.ttl("session");
    assert(ttl.has_value());
    assert(ttl->count() >= 0);
    assert(ttl->count() <= 1);

    std::this_thread::sleep_for(1100ms);

    assert(!store.get("session").has_value());
    assert(!store.contains("session"));
    assert(!store.ttl("session").has_value());
    assert(store.size() == 0);

    return 0;
}
