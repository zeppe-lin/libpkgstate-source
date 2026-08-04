# Architecture

## Authority flow

Contract shorthand: `sealed libpkgsource snapshot -> package_source_record`.

```text
sealed libpkgsource snapshot
+ caller-selected build architecture
+ caller-selected target architecture
                    |
                    v
             package_source_record
```

`libpkgstate-source` is an admission boundary. `libpkgsource` owns the sealed
recipe, its profile closure, source identities, lifecycle programs, and
architecture constraints. `libpkgstate` owns the durable installed-state
vocabulary. This repository translates between those two authorities and owns
neither endpoint.

## Admission invariants

`project_source()` accepts only a sealed `pkgsource::source_snapshot`; parser
syntax and YAML diagnostics are deliberately below this boundary. The caller
supplies the selected build and target architectures because selection is an
orchestration decision, not something state may infer from ambient hardware.
The adapter refuses selections outside the sealed recipe constraints.

A successful projection retains the exact package release, metadata, runtime
requirements, lifecycle programs and action-bound requirements, selected build
profiles, profile closure identities, architecture requirements, recipe
identity, and source-snapshot identity. Identity material is translated by its
canonical representation rather than re-derived from mutable source input.

## Non-authorities

The adapter does not discover source documents, parse YAML, expand profiles,
resolve dependencies, select architectures, fetch source material, execute a
build, inspect an image, apply files, read a state store, or publish state. A
failure is exported; no partial `package_source_record` is returned.

## Dependency placement

Both installed header dependencies are public because `project_source()`
exposes `libpkgsource` input types and a `libpkgstate` result type. There is no
planner, build, image, application, or persistence dependency.

The root commit records the exact 2.5.1 implementation and behavior test.
Subsequent commits may refine the independent adapter while the extraction
contract continues to verify the immutable origin rather than freezing current
implementation forever.
