#include "shard_test_suites.hpp"

int main() {
    using namespace minikv::shard_test;

    auto fixture = load_shard_fixture();
    run_shard_fixture_early_tests(fixture);
    run_shard_fixture_late_tests(fixture);
    run_shard_command_tests(fixture);
    return 0;
}
