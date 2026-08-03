# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
public_inc = include_directories('../include')
pkgstate_source = library(
  'pkgstate-source',
  [
    'adapter.cpp',
  ],
  cpp_args: [],
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
  requires_private: [],
)
