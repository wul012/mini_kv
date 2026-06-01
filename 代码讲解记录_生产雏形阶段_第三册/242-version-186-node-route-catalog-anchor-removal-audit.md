# mini-kv v186 Node route catalog anchor removal audit

v186 follows the latest Node v467 plan. It freezes v185 and extends the route/catalog compatibility window through Node v467, where Node removes compatibility source anchors from the route catalog test path. This is still evidence only: mini-kv does not import Node route catalogs, does not install an active router, and does not open write or execution paths.

## Implementation

`src/shard_readiness_node_compatibility.cpp` moves the compatibility window from Node v433-v464 to Node v433-v467. The audit still computes the numeric span from the same version vector, now proving count/span 35 and a contiguous, duplicate-free window.

`src/shard_readiness_route_catalog_closeout.cpp` adds `nodeRouteCatalogAnchorRemovalAudit`. It records that Node v467 removed source-anchor compatibility requirements, keeps central route-table behavior as a flatMap catalog consumer, and relies on typed catalog identity, route order, and flatMap alignment. The section also records Node v467 affected tests as 53 files / 59 tests plus typecheck/build pass markers.

`src/shard_readiness_history.cpp` adds the frozen v185 fixture and marks Node v465, v466, and v467 as archived evidence preserved. Historical fallback now points to v185, not rolling current.

`src/shard_readiness_boundary_fields.cpp` advances the catalog to `read-only-boundary-fields.v13`. The new group catalogs the anchor-removal audit's closed boundaries: no fresh mini-kv evidence consumption, no mini-kv route-anchor dependency, no runtime contract change, no services, no probes, no router, no writes, and no execution.

## Tests and Evidence

`tests/command_tests.cpp` and `tests/shard_readiness_tests.cpp` assert the v467 window, count/span 35, frozen v185 digest, anchor-removal audit fields, boundary catalog v13, and the new historical fixture. The frozen v185 fixture is also read back to prove the prior snapshot remains versioned.

Validation rebuilt the project, regenerated the rolling fixture, passed targeted `command_tests` and `shard_readiness_tests`, passed full CTest 71/71, and passed a TCP `SHARDJSON` smoke on `127.0.0.1:6459`. v186 gives Node's anchor-removal cleanup a mini-kv-side read-only evidence echo without coupling mini-kv to Node route internals.
