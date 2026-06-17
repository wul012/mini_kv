#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "test_support.hpp"

#include <cassert>

int main() {
    using minikv::test_support::assert_contains;

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SET order:100 paid");
    assert(result.response == "OK inserted");
    assert(store.size() == 1);

    result = processor.execute("GET order:100");
    assert(result.response == "paid");

    result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/prod.snap");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"LOAD\"");
    assert_contains(result.response, "\"write_command\":true");
    assert_contains(result.response, "\"store_replace_from_snapshot\"");
    assert_contains(result.response, "\"write command is not wal-backed\"");
    assert(store.size() == 1);

    result = processor.execute("GET order:100");
    assert(result.response == "paid");

    result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(result.response, "\"node_auto_start_allowed\":false");
    assert_contains(result.response, "\"java_auto_start_allowed\":false");
    assert_contains(result.response, "\"mini_kv_auto_start_allowed\":false");

    result = processor.execute("GET order:100");
    assert(result.response == "paid");
    assert(store.size() == 1);

    return 0;
}
