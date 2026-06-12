#!/usr/bin/env python3
"""Inventory mini-kv archive roots without moving historical evidence."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
import sys


DEFAULT_CEILINGS_MIB = {
    "a": 64.0,
    "b": 16.0,
    "c": 64.0,
    "d": 64.0,
    "e": 1200.0,
    "f": 128.0,
}


@dataclass(frozen=True)
class ArchiveRoot:
    name: str
    path: Path
    version_start: int | None
    version_end: int | None
    file_count: int
    byte_count: int
    ceiling_mib: float | None
    active: bool

    @property
    def mib(self) -> float:
        return self.byte_count / (1024 * 1024)

    @property
    def version_range(self) -> str:
        if self.version_start is None or self.version_end is None:
            return "-"
        if self.version_start == self.version_end:
            return str(self.version_start)
        return f"{self.version_start}-{self.version_end}"

    @property
    def status(self) -> str:
        if self.active:
            return "ACTIVE_ROOT_EXISTING_VERSIONS_FROZEN"
        return "FROZEN"


@dataclass(frozen=True)
class LatestVersionArchive:
    root_name: str
    version: int
    path: Path
    file_count: int
    byte_count: int

    @property
    def mib(self) -> float:
        return self.byte_count / (1024 * 1024)


def parse_root_ceiling(value: str) -> tuple[str, float]:
    if "=" not in value:
        raise argparse.ArgumentTypeError("expected ROOT=MIB, for example f=128")
    root, mib_text = value.split("=", 1)
    root = root.strip().lower()
    if len(root) != 1 or not root.isalpha():
        raise argparse.ArgumentTypeError("root must be a single letter")
    try:
        mib = float(mib_text)
    except ValueError as exc:
        raise argparse.ArgumentTypeError("ceiling MiB must be a number") from exc
    if mib <= 0:
        raise argparse.ArgumentTypeError("ceiling MiB must be positive")
    return root, mib


def numeric_version_dirs(path: Path) -> list[Path]:
    versions = [child for child in path.iterdir() if child.is_dir() and child.name.isdigit()]
    return sorted(versions, key=lambda child: int(child.name))


def count_files_and_bytes(path: Path) -> tuple[int, int]:
    file_count = 0
    byte_count = 0
    for child in path.rglob("*"):
        if child.is_file():
            file_count += 1
            byte_count += child.stat().st_size
    return file_count, byte_count


def discover_roots(repo: Path, ceilings: dict[str, float]) -> list[ArchiveRoot]:
    root_paths = [child for child in repo.iterdir() if child.is_dir() and len(child.name) == 1 and child.name.isalpha()]
    root_paths = sorted(root_paths, key=lambda child: child.name)
    active_root = root_paths[-1].name if root_paths else ""
    roots: list[ArchiveRoot] = []
    for root_path in root_paths:
        versions = numeric_version_dirs(root_path)
        file_count, byte_count = count_files_and_bytes(root_path)
        roots.append(
            ArchiveRoot(
                name=root_path.name,
                path=root_path,
                version_start=int(versions[0].name) if versions else None,
                version_end=int(versions[-1].name) if versions else None,
                file_count=file_count,
                byte_count=byte_count,
                ceiling_mib=ceilings.get(root_path.name),
                active=(root_path.name == active_root),
            )
        )
    return roots


def discover_latest_version(roots: list[ArchiveRoot]) -> LatestVersionArchive | None:
    latest_root: ArchiveRoot | None = None
    for root in roots:
        if root.version_end is None:
            continue
        if latest_root is None or root.version_end > (latest_root.version_end or -1):
            latest_root = root
    if latest_root is None or latest_root.version_end is None:
        return None
    version_path = latest_root.path / str(latest_root.version_end)
    file_count, byte_count = count_files_and_bytes(version_path)
    return LatestVersionArchive(
        root_name=latest_root.name,
        version=latest_root.version_end,
        path=version_path,
        file_count=file_count,
        byte_count=byte_count,
    )


def github_warning(enabled: bool, message: str) -> None:
    if enabled:
        print(f"::warning title=mini-kv archive inventory::{message}")


def render_table(roots: list[ArchiveRoot]) -> None:
    print("mini-kv archive inventory")
    print("root versions files bytes mib ceiling_mib status")
    for root in roots:
        ceiling = "-" if root.ceiling_mib is None else f"{root.ceiling_mib:.2f}"
        print(
            f"{root.name} {root.version_range} {root.file_count} {root.byte_count} "
            f"{root.mib:.2f} {ceiling} {root.status}"
        )


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path.cwd(), help="repository root")
    parser.add_argument("--budget-mib", type=float, default=8.0, help="newest archive version warning budget")
    parser.add_argument(
        "--root-ceiling-mib",
        action="append",
        default=[],
        type=parse_root_ceiling,
        metavar="ROOT=MIB",
        help="per-root warning ceiling; may be repeated",
    )
    parser.add_argument("--github-warning", action="store_true", help="emit GitHub Actions warning annotations")
    parser.add_argument("--strict", action="store_true", help="exit 1 when any warning is emitted")
    args = parser.parse_args(argv)

    ceilings = dict(DEFAULT_CEILINGS_MIB)
    ceilings.update(dict(args.root_ceiling_mib))

    repo = args.repo.resolve()
    roots = discover_roots(repo, ceilings)
    latest = discover_latest_version(roots)
    warnings: list[str] = []

    render_table(roots)
    if latest is not None:
        print(
            f"latest_version {latest.root_name}/{latest.version} files={latest.file_count} "
            f"bytes={latest.byte_count} mib={latest.mib:.2f} budget_mib={args.budget_mib:.2f}"
        )
        if latest.mib > args.budget_mib:
            warnings.append(
                f"latest archive {latest.root_name}/{latest.version} is {latest.mib:.2f} MiB "
                f"above the {args.budget_mib:.2f} MiB budget"
            )

    for root in roots:
        if root.ceiling_mib is not None and root.mib > root.ceiling_mib:
            warnings.append(
                f"archive root {root.name}/ is {root.mib:.2f} MiB above the "
                f"{root.ceiling_mib:.2f} MiB documented ceiling"
            )

    if warnings:
        print("warnings:")
        for warning in warnings:
            print(f"- {warning}")
            github_warning(args.github_warning, warning)
    else:
        print("warnings: none")

    return 1 if args.strict and warnings else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
