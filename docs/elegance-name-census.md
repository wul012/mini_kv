# v1659 Elegance Name Census

Status: E-M1 locally verified; commit, tag, push, and remote CI remain required before closure.

## Contract Definition

The budget is 40 characters. Filename census entries are regular-file stems under `src/`, `include/`, and `tests/`; extensions do not count. Public-header identifier entries are unique C++ identifier tokens visible in `include/` headers after `#include` paths, comments, ordinary/raw string literals, and character literals are removed. This deliberately avoids counting local implementation variables while still exposing all names carried by the public header surface.

The committed baseline is exact, not merely a ceiling. A current entry absent from the baseline is a new violation; a baseline entry absent from the current tree is stale and must be removed in the same commit. CMake also compiles fixed maxima into the scanner, preventing a baseline edit from silently increasing either debt count.

## E-M1 Baseline

| Category | Budget | Initial count | Hard maximum | Policy |
|---|---:|---:|---:|---|
| filename stems | 40 | 740 | 740 | exact set, shrink only |
| public-header identifiers | 40 | 883 | 883 | exact set, shrink only |

The baseline lives at `config/elegance-name-baseline.txt`; the scanner and CTest entry are `tests/elegance_name_census_tests.cpp` and `elegance_name_census_contract`.

## Requirement-To-Evidence Matrix

| Requirement | Implementation | Failing evidence | E-M1 state |
|---|---|---|---|
| Freeze legacy debt | deterministic `F|` and `I|` baseline rows | deleting or adding a row makes exact-set comparison fail | closed locally |
| Reject new long filenames | recursive stem census plus 740 maximum | temporary 741st long filename fails with its exact path | closed locally |
| Reject new public-header long identifiers | literal/comment-aware header scan plus 883 maximum | any new token is reported as an added identifier | closed locally |
| Require same-commit tightening | stale baseline entries are errors | a renamed/removed violation fails until baseline and maximum shrink | closed locally |
| Preserve contracts | scanner is a standalone test executable | 351/351 CTest plus 1514-file fixture manifest SHA-256 | closed locally |

## Reproduction

```powershell
cmake --build cmake-build-v1659 --target minikv_elegance_name_census
ctest --test-dir cmake-build-v1659 -R '^elegance_name_census_contract$' --output-on-failure
cmake-build-v1659\minikv_elegance_name_census.exe --list
```

This version does not rename a public header, public symbol, CLI command, RESP/JSON key, fixture, or archived path. The top-five internal-file cleanup belongs to the next version and must lower the 740 maximum in the same commit.
