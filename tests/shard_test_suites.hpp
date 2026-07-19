#pragma once

#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "test_support.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

namespace minikv::shard_test {

struct ShardFixture {
    std::string current;
    std::map<int, std::string> historical;
    std::string formatted;
    const std::string* active_json = nullptr;

    const std::string& version(int release_version) const { return historical.at(release_version); }
};

using test_support::assert_contains;
using test_support::read_fixture_text;

ShardFixture load_shard_fixture();
void assert_shard_readiness_contract(ShardFixture& fixture, const std::string& json);
void check_shard_candidate_contract(ShardFixture& fixture);
void check_shard_governance_contract(ShardFixture& fixture);
void check_shard_route_contract(ShardFixture& fixture);
void check_shard_history_contract(ShardFixture& fixture);
void check_shard_catalog_contract(ShardFixture& fixture);
void run_shard_fixture_early_tests(ShardFixture& fixture);
void run_shard_fixture_late_tests(ShardFixture& fixture);
void run_shard_command_tests(ShardFixture& fixture);

} // namespace minikv::shard_test
