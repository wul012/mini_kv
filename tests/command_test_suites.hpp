#pragma once

#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "minikv/wal.hpp"
#include "test_support.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <string>
#include <string_view>
#include <thread>

namespace minikv::command_test {

struct CommandFixture {
    Store store;
    CommandProcessor processor{store};
    CommandResult result;
};

using test_support::assert_response_contains;
using test_support::read_fixture_text;

void run_command_core_tests(CommandFixture& fixture);
void run_command_shard_route_tests(CommandFixture& fixture);
void run_command_shard_material_tests(CommandFixture& fixture);
void run_command_shard_receipt_tests(CommandFixture& fixture);
void run_command_shard_supply_tests(CommandFixture& fixture);
void run_command_shard_compat_tests(CommandFixture& fixture);
void run_command_shard_freeze_tests(CommandFixture& fixture);
void run_command_shard_closeout_tests(CommandFixture& fixture);
void run_command_runtime_tests(CommandFixture& fixture);

} // namespace minikv::command_test
