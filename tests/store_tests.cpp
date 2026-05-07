#include "minikv/store.hpp"

#include <cassert>
#include <string>

int main() {
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

    return 0;
}
