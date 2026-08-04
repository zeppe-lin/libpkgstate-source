# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
scdoc = find_program('scdoc', required: get_option('man_pages'))
if scdoc.found()
  manual = custom_target(
    'libpkgstate-source.3',
    input: 'libpkgstate-source.3.scdoc',
    output: 'libpkgstate-source.3',
    command: [scdoc],
    capture: true,
    install: true,
    install_dir: get_option('mandir') / 'man3',
  )
endif
