#include "minikv/route_preview_evidence_boundary.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    const std::string fields = minikv::route_preview_evidence_boundary::format_no_execution_boundary_fields();

    assert(fields.rfind(",", 0) == 0);
    minikv::test_support::assert_contains(fields, "\"readOnly\":true");
    minikv::test_support::assert_contains(fields, "\"mutatesStore\":false");
    minikv::test_support::assert_contains(fields, "\"touchesWal\":false");
    minikv::test_support::assert_contains(fields, "\"filesystemReadPerformed\":false");
    minikv::test_support::assert_contains(fields, "\"runtimeArchiveWalkAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"executesRoute\":false");
    minikv::test_support::assert_contains(fields, "\"storageDirectoriesCreated\":false");
    minikv::test_support::assert_contains(fields, "\"activeRouterInstalled\":false");
    minikv::test_support::assert_contains(fields, "\"routerActivationAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"writeRoutingAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"writeCommandsAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"adminCommandsAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"loadRestoreCompactAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"startsMiniKvService\":false");
    minikv::test_support::assert_contains(fields, "\"startsServices\":false");
    minikv::test_support::assert_contains(fields, "\"runtimeProbeAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"liveReadAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(fields, "\"executionAllowed\":true");

    return 0;
}
