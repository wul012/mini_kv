# Elegance Name Census

Status: E-M1 gate shipped in v1659; E-M2 top-five cleanup and Windows CRLF portability repair are locally implemented in v1660.

## Contract Definition

The budget is 40 characters. Filename census entries are regular-file stems under `src/`, `include/`, and `tests/`; extensions do not count. Public-header identifier entries are unique C++ identifier tokens visible in `include/` headers after `#include` paths, comments, ordinary/raw string literals, and character literals are removed. This deliberately avoids counting local implementation variables while still exposing all names carried by the public header surface.

The committed baseline is exact, not merely a ceiling. A current entry absent from the baseline is a new violation; a baseline entry absent from the current tree is stale and must be removed in the same commit. CMake also compiles fixed maxima into the scanner, preventing a baseline edit from silently increasing either debt count.

## E-M1 Baseline

| Category | Budget | Initial count | Current hard maximum | Policy |
|---|---:|---:|---:|---|
| filename stems | 40 | 740 | 735 | exact set, shrink only |
| public-header identifiers | 40 | 883 | 883 | exact set, shrink only |

## Ratchet History

| Version | Long filenames | Public-header identifiers | Change |
|---|---:|---:|---|
| v1659 | 740 | 883 | initial exact baseline and red-path proof |
| v1660 | 735 | 883 | five longest unpinned internal `.cpp` paths renamed; CRLF baseline parsing fixed; public surface unchanged |

v1659 GitHub Actions run `29176190797` passed six jobs but failed `windows-latest`: Git checkout produced CRLF baseline lines and the parser retained each trailing carriage return. v1660 strips exactly one terminal `\r` after `std::getline` and executes an in-memory CRLF baseline sample on every run. The fix does not normalize identifiers, paths, case, or whitespace inside entries.

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

v1660 does not rename a public header, public symbol, CLI command, RESP/JSON key, fixture, or archived path. It shortens only five CMake-private implementation paths and lowers the filename maximum to 735 in the same commit; the 883 identifier maximum remains unchanged.
