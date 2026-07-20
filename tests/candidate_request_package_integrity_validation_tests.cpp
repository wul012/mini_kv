#include "minikv/shard_preview_candidate_request_package_integrity_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_request_package_integrity_validation;

    const auto passed =
        validation::format_candidate_request_package_integrity_validation_json(15, true, 25, 25, 25, 25);
    assert_contains(passed, "\"candidateRequestPackageIntegrityValidationPassed\":true");
    assert_contains(passed, "\"sourcePackageFrozen\":true");
    assert_contains(passed, "\"integrityCountsAligned\":true");
    assert_contains(passed, "\"additionalRequestEchoCreated\":false");
    assert_contains(passed, "\"documentIntakeOpened\":false");
    assert_contains(passed, "\"documentRouterInstalled\":false");
    assert_contains(passed, "\"writeRoutingAllowed\":false");
    assert_contains(passed, "\"touchesWal\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed =
        validation::format_candidate_request_package_integrity_validation_json(14, true, 25, 25, 25, 25);
    assert_contains(failed, "\"candidateRequestPackageIntegrityValidationPassed\":false");
    assert_contains(failed, "\"sourcePackageFrozen\":false");
    return 0;
}
