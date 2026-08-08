#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
fail(){ echo "test-layout-contract: $*" >&2; exit 1; }

for dir in contracts fixtures header integration support unit; do
  [ -d "$root/tests/$dir" ] || fail "missing tests/$dir"
done

if find "$root/tests" -maxdepth 1 -type f \( -name '*.cpp' -o -name '*.h' -o -name 'check_*.sh' \) | grep . >/dev/null; then
  fail 'uncategorized test source remains in tests root'
fi

for file in \
  tests/fixtures/source.h \
  tests/support/test.h \
  tests/support/record_query.h \
  tests/unit/projection_error_test.cpp \
  tests/header/public_header_test.cpp \
  tests/integration/release_metadata_test.cpp \
  tests/integration/minimal_source_test.cpp \
  tests/integration/requirement_provenance_test.cpp \
  tests/integration/lifecycle_test.cpp \
  tests/integration/profile_selection_test.cpp \
  tests/integration/architecture_test.cpp \
  tests/integration/scope_test.cpp; do
  [ -s "$root/$file" ] || fail "missing $file"
done

grep -F "suite: 'unit'" "$root/tests/meson.build" >/dev/null ||
  fail 'unit suite is not registered'
grep -F "suite: 'integration'" "$root/tests/meson.build" >/dev/null ||
  fail 'integration suite is not registered'
grep -F "suite: 'header'" "$root/tests/meson.build" >/dev/null ||
  fail 'header suite is not registered'
grep -F "suite: 'contract'" "$root/tests/meson.build" >/dev/null ||
  fail 'contract suite is not registered'
