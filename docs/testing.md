# Testing

Qualification is separated by evidence role so one broad behavior executable
cannot hide which side of the source-to-state seam failed.

## Qualification roles

- `tests/unit/` covers adapter-owned values that require no source sealing:
  typed projection errors and their stable refusal codes.
- `tests/integration/` constructs genuine sealed `libpkgsource` snapshots and
  projects them through the public adapter into real `libpkgstate` records.
  No source or state model is mocked.
- `tests/fixtures/` contains deterministic source/profile fiction built only
  through public `libpkgsource` APIs. Fixtures manufacture input authority; they
  do not reproduce adapter policy.
- `tests/support/` contains assertion and record-query helpers only.
- `tests/contracts/` checks architecture placement, ABI/pkg-config/release
  metadata, documentation closure, extraction provenance, repository/CI/style
  rules, and the test topology itself.
- `tests/header/` contains the single standalone-header harness; the `header`
  suite compiles every installed header independently under the declared public
  dependency closure.

Caller-side persistence and orchestration tests belong in callers such as
`libpkgstate-posix` and `pkgctl`. This repository proves the callee seam with
its two real semantic owners rather than adding an upward dependency on a
controller.

## Behavioral matrix

The integration suite proves:

- minimal source authority with absent optional metadata and no requirements, lifecycle control, selected profiles, or architecture restrictions projects without invented facts;
- package release coordinates and the foreign release identity are translated
  exactly rather than re-derived under a state-owned identity domain;
- summary, multiline description, homepage, normalized license order, and the
  complete foreign source-snapshot identity survive projection;
- runtime requirements preserve direct and profile-expanded provenance,
  including package/profile member kind, every traversed profile reference,
  exact member text, and declaration document/path/line/column;
- all four lifecycle actions retain their exact POSIX-shell programs and
  action-bound requirement provenance;
- selected build-profile roots retain their exact source-owned profile
  identities and issuing declarations;
- declared build and target architecture sets remain distinct, closed sets
  reject each non-authorized selection independently, and open declarations
  accept and retain caller-selected architectures;
- build and check requirements do not leak into durable runtime or lifecycle
  control, while selected build-profile evidence is retained; and
- equivalent sealed authority plus equivalent architecture selections produces
  the same canonical `package_source_record` identity.

The unit suite proves the stable projection refusal type independently of the
endpoint models. Endpoint-invalid source objects are not manufactured merely to
exercise unreachable branches: validity of a sealed source snapshot belongs to
`libpkgsource`, while validity of a native state record belongs to
`libpkgstate`.

## Running

Run the complete project qualification:

```sh
meson test -C build --print-errorlogs
```

Run one evidence class while diagnosing a failure:

```sh
meson test -C build --suite unit --print-errorlogs
meson test -C build --suite integration --print-errorlogs
meson test -C build --suite header --print-errorlogs
meson test -C build --suite contract --print-errorlogs
```

Shared and static configurations still require separate build directories. CI
runs GCC and Clang, release mode, and ASan/UBSan qualification. Installation
qualification compiles a consumer against staged headers and metadata, audits
the shared/static dependency boundary, and verifies installed manuals and
project documentation.

The documentation contract receives the include roots of the production
dependencies resolved by Meson before Clang parses public headers. Ambient
system-installed zoo headers are not accepted as dependency closure.

A release candidate is incomplete until the exact dependency tags used by CI
exist and the entire matrix is green from clean build directories.
