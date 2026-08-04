# libpkgstate-source

libpkgstate-source provides durable admission of sealed package-source authority.

```text
sealed libpkgsource snapshot + selected architectures -> package_source_record
```

## Authority

This repository owns the translation between a sealed `libpkgsource` snapshot and the state-owned `package_source_record` vocabulary. It is a translation boundary, not another semantic owner. Its input and output models remain authoritative in their respective repositories.

The public operation accepts one sealed source snapshot and caller-selected build and target architectures. A successful projection retains release and metadata, runtime and lifecycle requirements, lifecycle programs, selected profiles, architecture constraints, and exact recipe and snapshot identities.

The adapter performs no discovery, parsing, dependency resolution, build execution, archive inspection, target mutation, state publication, migration, retry policy, or compatibility import unless the operation is explicitly part of the contract above. It exports refusal rather than guessing. It refuses identity vocabulary drift, unknown enum vocabulary, architecture selections outside sealed constraints, and state record construction failure.

See `docs/architecture.md` for invariants and `docs/integration.md` for placement in the package-management graph.

## Dependency boundary

Public installed closure: `libpkgstate >=3.0.0` and `libpkgsource >=3.0.0`.

Private implementation closure: none.

Fallback subprojects are intentionally unsupported. Shared consumers receive only public requirements; static consumers receive the complete private closure through pkg-config.

## Build

```sh
meson setup build-shared \
  -Ddefault_library=shared \
  -Dlink_mode=shared
meson compile -C build-shared
meson test -C build-shared --print-errorlogs

meson setup build-static \
  -Ddefault_library=static \
  -Dlink_mode=static
meson compile -C build-static
meson test -C build-static --print-errorlogs
```

Shared and static artifacts must come from separate build directories. `default_library=both` is rejected because one dependency closure cannot truthfully represent both linkage modes.

## Release lineage

The 3.0 repository was extracted from `libpkgstate` 2.5.1. The repository root preserves extraction provenance; later commits may evolve the independent product without rewriting that history. The library preserves SONAME generation 1.

Release after `libpkgstate` 3.0.0 and `libpkgsource` 3.0.0. Release before `libpkgstate-build` and `libpkgstate-apply`.

## Documentation

- `docs/architecture.md` — authority and refusal invariants;
- `docs/integration.md` — composition and release order;
- `docs/testing.md` — qualification matrix;
- `docs/abi.md` — ABI and pkg-config policy;
- `MAINTAINING.md` — release gate.

## License

GPL-3.0-or-later. See `COPYING` and `COPYRIGHT`.
