#include "minikv/shard_preview_catalog_entry_group_split_non_participation_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_catalog_entry_group_split_non_participation_validation;

    const auto passed = validation::format_catalog_entry_group_split_non_participation_validation_json(35, true, false, false, false, false, 35, 35, 35, 35);
    assert_contains(passed, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":true");
    assert_contains(passed, "\"sourceLocked\":true");
    assert_contains(passed, "\"catalogSplitClosed\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"miniKvCatalogSplitStarted\":false");
    assert_contains(passed, "\"catalogEndpointRead\":false");
    assert_contains(passed, "\"catalogRecordsImported\":false");
    assert_contains(passed, "\"nodeCatalogRegistryExecutedByMiniKv\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_source =
        validation::format_catalog_entry_group_split_non_participation_validation_json(34, true, false, false, false, false, 35, 35, 35, 35);
    assert_contains(failed_source, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_source, "\"sourceLocked\":false");

    const auto failed_closure =
        validation::format_catalog_entry_group_split_non_participation_validation_json(35, true, false, true, false, false, 35, 35, 35, 35);
    assert_contains(failed_closure, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_closure, "\"catalogSplitClosed\":false");

    const auto failed_count =
        validation::format_catalog_entry_group_split_non_participation_validation_json(35, true, false, false, false, false, 35, 34, 35, 35);
    assert_contains(failed_count, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":false");
    assert_contains(failed_count, "\"countsAligned\":false");

    return 0;
}
