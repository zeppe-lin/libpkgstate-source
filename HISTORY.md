# History

## 3.0.0 (2026-08-04)

- Extracted `libpkgstate-source` from the `libpkgstate` 2.5.1 repository.
- Preserved the existing adapter behavior and SONAME generation 1.
- Established an independent dependency closure: libpkgstate >=3.0.0; libpkgsource >=3.0.0.
- Added public-header, pkg-config, extraction-provenance, architecture, repository, compiler, sanitizer, shared, and static qualification.
- Classify architecture-selection refusal structurally instead of inspecting diagnostic text.
