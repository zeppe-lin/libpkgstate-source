# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
public_inc = include_directories('../include')

elf_export_generator = find_program(
  meson.project_source_root() / 'tools/generate-elf-export-script.sh',
)
export_script_name = 'libpkgstate-source.exports.map'
export_script_path = meson.current_build_dir() / export_script_name
export_script = configure_file(
  input: '../abi/libpkgstate-source.exports',
  output: export_script_name,
  command: [elf_export_generator, '@INPUT@'],
  capture: true,
)

pkgstate_source = library(
  'pkgstate-source',
  [
    'adapter.cpp',
  ],
  cpp_args: visibility_cpp_args + ['-DPKGSTATE_SOURCE_BUILDING_LIBRARY'],
  gnu_symbol_visibility: 'hidden',
  link_args: ['-Wl,--version-script=' + export_script_path],
  link_depends: [export_script],
  include_directories: public_inc,
  dependencies: [
    libpkgstate_dep,
    libpkgsource_dep,
  ],
  install: true,
  version: meson.project_version(),
  soversion: '1',
)
libpkgstate_source_dep = declare_dependency(
  link_with: pkgstate_source,
  include_directories: public_inc,
  dependencies: [
    libpkgstate_dep,
    libpkgsource_dep,
  ],
  compile_args: [],
  version: meson.project_version(),
)
meson.override_dependency('libpkgstate-source', libpkgstate_source_dep)
install_headers(
  '../include/libpkgstate-source/export.h',
  '../include/libpkgstate-source/adapter.h',
  '../include/libpkgstate-source/libpkgstate-source.h',
  subdir: 'libpkgstate-source',
)
pkgconf.generate(
  pkgstate_source,
  name: 'libpkgstate-source',
  filebase: 'libpkgstate-source',
  description: 'Projection from sealed package-source authority into libpkgstate',
  subdirs: '.',
  requires: [libpkgstate_dep, libpkgsource_dep],
)
