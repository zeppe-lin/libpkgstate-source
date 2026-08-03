# Integration

Call `project_source()` after source authority has been sealed and after the
orchestrator has selected exact build and target architectures. Retain the
returned `package_source_record` as the only state-facing source projection;
do not reproduce it from filenames, planner candidates, or parser documents.

```text
libpkgsource seal
       |
architecture selection (caller)
       |
libpkgstate-source admission
       |
package_source_record
```

Link this product only at that composition point. It performs no I/O and has no
fallback subprojects. Release `libpkgstate` 3.0.0 and `libpkgsource` 3.0.0
first; release `libpkgstate-source` before `libpkgstate-build` and
`libpkgstate-apply`.
