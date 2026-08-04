# History

## 3.0.0 (2026-08-04)

- Extracted `libpkgstate-source` from the `libpkgstate` 2.5.1 repository.
- Preserved the extracted behavior as repository provenance and retained SONAME generation 1.
- Established an independent dependency closure: libpkgstate >=3.0.0; libpkgsource >=3.0.0.
- Added public-header, pkg-config, extraction-provenance, architecture, repository, compiler, sanitizer, shared, and static qualification.
- Completed the documented public projection contract under Doxygen warnings-as-errors.
- Renamed the installed manual from the former in-tree adapter name to `libpkgstate-source.3`.
- Classify architecture-selection refusal structurally instead of inspecting diagnostic text.
