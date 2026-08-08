# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
if get_option('tests').allowed()
  unit_tests = [
    ['projection-error', 'unit/projection_error_test.cpp'],
  ]
  foreach item : unit_tests
    unit_test = executable(
      item[0] + '-test',
      item[1],
      dependencies: libpkgstate_source_dep,
      cpp_args: ['-UNDEBUG'],
    )
    test(item[0], unit_test, suite: 'unit')
  endforeach

  integration_tests = [
    ['release-metadata', 'integration/release_metadata_test.cpp'],
    ['minimal-source', 'integration/minimal_source_test.cpp'],
    ['requirement-provenance', 'integration/requirement_provenance_test.cpp'],
    ['lifecycle', 'integration/lifecycle_test.cpp'],
    ['profile-selection', 'integration/profile_selection_test.cpp'],
    ['architecture', 'integration/architecture_test.cpp'],
    ['scope', 'integration/scope_test.cpp'],
  ]
  foreach item : integration_tests
    integration_test = executable(
      item[0] + '-integration-test',
      item[1],
      dependencies: libpkgstate_source_dep,
      cpp_args: ['-UNDEBUG'],
    )
    test(item[0], integration_test, suite: 'integration')
  endforeach

  public_headers = [
    ['export', 'libpkgstate-source/export.h'],
    ['adapter', 'libpkgstate-source/adapter.h'],
    ['libpkgstate_source', 'libpkgstate-source/libpkgstate-source.h'],
  ]
  foreach header : public_headers
    public_header = executable(
      header[0] + '-public-header-test',
      'header/public_header_test.cpp',
      dependencies: libpkgstate_source_dep,
      cpp_args: ['-DLIBPKGSTATE_SOURCE_TEST_HEADER="' + header[1] + '"'],
    )
    test(
      'compile public header ' + header[1],
      public_header,
      suite: 'header',
    )
  endforeach

  sh = find_program('sh')
  if library_kind == 'shared'
    nm = find_program('nm')
    test(
      'verify reviewed adapter ABI surface',
      sh,
      args: [
        join_paths(meson.current_source_dir(), 'contracts/check_abi_surface.sh'),
        pkgstate_source.full_path(),
        meson.project_source_root() / 'abi/libpkgstate-source.exports',
        nm.full_path(),
      ],
      suite: 'contract',
    )
  endif

  libpkgstate_public_include = libpkgstate_dep.get_variable(
    pkgconfig: 'includedir',
  )
  libpkgsource_public_include = libpkgsource_dep.get_variable(
    pkgconfig: 'includedir',
  )
  foreach contract : [
    'architecture',
    'documentation',
    'release_metadata',
    'repository',
    'extraction',
    'ci',
    'test_layout',
  ]
    contract_args = [
      join_paths(meson.current_source_dir(), 'contracts/check_' + contract + '.sh'),
      meson.project_source_root(),
    ]
    if contract == 'documentation'
      contract_args += [
        libpkgstate_public_include,
        libpkgsource_public_include,
      ]
    endif
    test(
      contract.replace('_', '-'),
      sh,
      args: contract_args,
      suite: 'contract',
    )
  endforeach
  test(
    'style-contract',
    sh,
    args: [
      join_paths(meson.current_source_dir(), 'contracts/check_style.sh'),
      meson.project_source_root(),
      meson.project_build_root(),
    ],
    suite: 'contract',
  )
  test(
    'pkgconfig-metadata',
    sh,
    args: [
      join_paths(meson.current_source_dir(), 'contracts/check_pkgconfig_metadata.sh'),
      meson.project_build_root(),
    ],
    suite: 'contract',
  )
endif
