#include "minikv/shard_preview_candidate_request_package_checks.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_request_package_checks;
    const auto records = checks::candidate_document_acceptance_checks();

    assert(records.size() == 15);
    assert(checks::planned_candidate_document_acceptance_check_count() == 15);
    assert(records.front().check_code == std::string{"source_intake_contract_check"});
    assert(records.back().check_code == std::string{"archive_closeout_check"});

    const auto none = checks::format_candidate_document_acceptance_checks_json(0);
    assert(none == "[]");

    const auto all = checks::format_candidate_document_acceptance_checks_json(99);
    assert_contains(all, "\"checkCode\":\"archive_closeout_check\"");
    assert_contains(all, "\"passed\":true");
    assert_contains(all, "\"documentAccepted\":false");
    assert_contains(all, "\"executionAllowed\":false");
}
