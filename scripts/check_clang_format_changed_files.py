#!/usr/bin/env python3
"""Run clang-format only on changed C/C++ files."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path


FORMATTED_EXTENSIONS = {
    ".c",
    ".cc",
    ".cpp",
    ".cxx",
    ".h",
    ".hh",
    ".hpp",
    ".hxx",
}


def run_git(args: list[str]) -> str:
    completed = subprocess.run(["git", *args], check=True, text=True, stdout=subprocess.PIPE)
    return completed.stdout


def first_parent() -> str:
    return run_git(["rev-list", "--max-parents=0", "HEAD"]).splitlines()[0]


def resolve_base(base: str) -> str:
    if not base or set(base) == {"0"}:
        try:
            return run_git(["rev-parse", "HEAD~1"]).strip()
        except subprocess.CalledProcessError:
            return first_parent()
    try:
        return run_git(["rev-parse", "--verify", base]).strip()
    except subprocess.CalledProcessError:
        return first_parent()


def changed_files(base: str, head: str) -> list[Path]:
    diff_output = run_git(["diff", "--name-only", "--diff-filter=ACMRT", base, head, "--"])
    files: list[Path] = []
    for line in diff_output.splitlines():
        path = Path(line)
        if path.suffix.lower() in FORMATTED_EXTENSIONS and path.is_file():
            files.append(path)
    return files


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", default="HEAD~1", help="base git revision for changed-file detection")
    parser.add_argument("--head", default="HEAD", help="head git revision for changed-file detection")
    parser.add_argument("--clang-format", default="clang-format", help="clang-format executable")
    args = parser.parse_args()

    formatter = shutil.which(args.clang_format)
    if formatter is None:
        print(f"clang-format executable not found: {args.clang_format}", file=sys.stderr)
        return 2

    base = resolve_base(args.base)
    files = changed_files(base, args.head)
    if not files:
        print("No changed C/C++ files to format-check.")
        return 0

    print("clang-format checking changed files:")
    for path in files:
        print(f"  {path.as_posix()}")

    subprocess.run([formatter, "--dry-run", "--Werror", *[str(path) for path in files]], check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
