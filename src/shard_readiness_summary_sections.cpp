#include "shard_readiness_summary_sections.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::summary_sections {

std::string format_notes_json() {
    return runtime_evidence::json_string_array({
        "read-only shard readiness hardening",
        "single logical shard only",
        "slot table is evidence, not active storage routing",
        "does not create shard directories or start extra processes",
        "freezes v155 approval gate input precheck",
        "declares operator lifecycle evidence without runtime gate approval",
        "adds mini-kv runtime execution artifact candidate without approval",
        "adds runtime execution approval gate input precheck with zero of three inputs",
        "adds final mini-kv approval gate input while global runtime remains blocked",
        "freezes v156 final mini-kv approval gate input",
        "echoes Node v402 templates as non-canonical approval inputs",
        "freezes v157 template validator echo",
        "adds canonical approval input precheck with zero of three inputs",
        "adds Node v418 route-group split compatibility evidence",
        "splits read-only boundary field catalog without changing SHARDJSON command or fixture path",
        "extends boundary catalog index to v10 for numeric route window audit fields without changing runtime permissions",
        "adds read-only slot table preview without installing an active router",
        "adds read-only slot table preview consistency audit without activating routing",
        "splits slot preview audit formatter without changing command or runtime boundaries",
        "adds versioned shard readiness release catalog without runtime permissions",
        "adds release catalog consistency audit without runtime permissions",
        "keeps Node route split compatibility window through v464 without runtime permissions",
        "snapshots Node v464 route catalog final closeout validation without consuming fresh mini-kv evidence",
        "audits Node v467 route catalog anchor removal without adding mini-kv route-anchor dependency",
        "audits Node v471 expected integrity snapshot centralization without making mini-kv a catalog integrity authority",
        "freezes v187 expected integrity snapshot audit evidence without using rolling current as the baseline",
        "catalogs v185-v188 route catalog evidence chain from versioned fixtures without runtime permissions",
        "audits Node v472 route catalog cleanup closeout validation without adding routes or runtime gates",
        "hands off the v190 cleanup closeout audit through a versioned read-only fixture",
        "audits the v191 cleanup closeout handoff manifest without opening runtime behavior",
        "freezes the v192 cleanup closeout handoff audit as versioned read-only evidence",
        "catalogs v190-v193 cleanup closeout releases from versioned fixtures without runtime permissions",
        "audits the v194 cleanup closeout release catalog for continuity and monotonic evidence counts",
        "splits cleanup closeout catalog formatters without changing catalog or audit sections",
        "packages v194-v196 latest route catalog cleanup evidence for archive verification",
        "audits the v197 latest route catalog cleanup evidence package from a frozen fixture",
        "closes out v194-v198 cleanup evidence as a read-only batch aligned with Node v480",
        "audits the v199 cleanup evidence batch closeout from a frozen fixture",
        "starts a post-closeout continuity ledger from the frozen v200 audit fixture",
        "computes Node route split window contiguity and duplicate checks from the version vector without runtime permissions",
        "computes Node route split window numeric span and count matching without runtime permissions",
        "splits shard readiness history formatters without changing command or runtime boundaries",
        "runtime execution artifact intake preflight remains blocked at 0 of 6 artifacts",
        "live-read gate remains prerequisite-only and does not start services",
        "operator service lifecycle evidence still has no runtime probe",
        "active shard prototype remains plan-prerequisite only",
        "does not mutate Node v370-v480 archived evidence",
        "not order or audit authoritative",
    });
}

} // namespace minikv::shard_readiness::summary_sections
