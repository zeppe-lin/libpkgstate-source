#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
build=$1
pc=$build/meson-private/libpkgstate-source.pc
[ -s "$pc" ] || { echo "pkgconfig-metadata: missing $pc" >&2; exit 1; }
grep -F 'Version: 3.0.0' "$pc" >/dev/null
grep -F -- '-lpkgstate-source' "$pc" >/dev/null
public=$(sed -n 's/^Requires:[[:space:]]*//p' "$pc")
private=$(sed -n 's/^Requires\.private:[[:space:]]*//p' "$pc")
private_libs=$(sed -n 's/^Libs\.private:[[:space:]]*//p' "$pc")
printf '%s\n' "$public" | grep -F 'libpkgstate >=3.0.0' >/dev/null || { echo 'pkgconfig-metadata: missing public libpkgstate >=3.0.0' >&2; exit 1; }
printf '%s\n' "$public" | grep -F 'libpkgsource >=3.0.0' >/dev/null || { echo 'pkgconfig-metadata: missing public libpkgsource >=3.0.0' >&2; exit 1; }
if printf '%s\n' "$public" | grep -F 'libpkgstate-source' >/dev/null; then echo 'pkgconfig-metadata: private edge leaked publicly: libpkgstate-source' >&2; exit 1; fi
