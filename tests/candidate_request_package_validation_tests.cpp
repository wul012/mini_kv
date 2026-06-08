#include "minikv/shard_preview_candidate_request_package_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_request_package_validation;

    const auto ok = validation::format_candidate_document_request_package_validation_json(
        15, 15, 38, 1, 1, 3, true, 1);
    assert_contains(ok, "\"candidateDocumentRequestPackageValidationPassed\":true");
    assert_contains(ok, "\"closeoutComplete\":false");
    assert_contains(ok, "\"completedItemCount\":1");
    assert_contains(ok, "\"completedCheckCount\":1");
    assert_contains(ok, "\"completedGateCount\":3");
    assert_contains(ok, "\"realCandidateDocumentAccepted\":false");
    assert_contains(ok, "\"payloadImported\":false");
    assert_contains(ok, "\"executionAllowed\":false");

    const auto complete = validation::format_candidate_document_request_package_validation_json(
        15, 15, 38, 15, 15, 38, true, 15);
    assert_contains(complete, "\"candidateDocumentRequestPackageValidationPassed\":true");
    assert_contains(complete, "\"closeoutComplete\":true");

    const auto blocked = validation::format_candidate_document_request_package_validation_json(
        15, 15, 38, 2, 1, 4, true, 2);
    assert_contains(blocked, "\"candidateDocumentRequestPackageValidationPassed\":false");
    assert_contains(blocked, "\"itemCheckCountsAligned\":false");
}
