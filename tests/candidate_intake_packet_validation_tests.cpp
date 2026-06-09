#include "minikv/shard_preview_candidate_intake_packet_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_intake_packet_validation;

    const auto passed = validation::format_candidate_intake_packet_validation_json(10, true, 10, 10, 10, 10, 25, 25);
    assert_contains(passed, "\"candidateIntakePacketValidationPassed\":true");
    assert_contains(passed, "\"sourcePrecheckFrozen\":true");
    assert_contains(passed, "\"slotCountsAligned\":true");
    assert_contains(passed, "\"guardCountsAligned\":true");
    assert_contains(passed, "\"stageCountsAligned\":true");
    assert_contains(passed, "\"additionalIntakeEchoCreated\":false");
    assert_contains(passed, "\"documentIntakeOpened\":false");
    assert_contains(passed, "\"documentRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed = validation::format_candidate_intake_packet_validation_json(9, true, 10, 10, 10, 10, 25, 25);
    assert_contains(failed, "\"candidateIntakePacketValidationPassed\":false");
    assert_contains(failed, "\"sourcePrecheckFrozen\":false");
}
