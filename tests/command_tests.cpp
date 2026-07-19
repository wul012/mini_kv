#include "command_test_suites.hpp"

int main() {
    using namespace minikv::command_test;

    CommandFixture fixture;
    run_command_core_tests(fixture);
    run_command_shard_route_tests(fixture);
    run_command_shard_material_tests(fixture);
    run_command_shard_receipt_tests(fixture);
    run_command_shard_supply_tests(fixture);
    run_command_shard_compat_tests(fixture);
    run_command_shard_freeze_tests(fixture);
    run_command_shard_closeout_tests(fixture);
    run_command_runtime_tests(fixture);
    return 0;
}
