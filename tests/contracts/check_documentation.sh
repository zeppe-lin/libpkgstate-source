#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
fail(){ echo "documentation-contract: $*" >&2; exit 1; }
state_include=${2:-}
source_include=${3:-}

resolve_include()
{
  package=$1
  value=$2
  if [ -n "$value" ]; then
    printf '%s\n' "$value"
    return
  fi
  command -v pkg-config >/dev/null 2>&1 ||
    fail "$package include root is unavailable"
  pkg-config --exists "$package" ||
    fail "$package include root is unavailable"
  pkg-config --variable=includedir "$package"
}

state_include=$(resolve_include libpkgstate "$state_include")
source_include=$(resolve_include libpkgsource "$source_include")
for file in \
  README.md HISTORY.md CONTRIBUTING.md MAINTAINING.md Doxyfile \
  docs/architecture.md docs/integration.md docs/testing.md docs/abi.md \
  docs/code-style.md docs/meson.build man/libpkgstate-source.3.scdoc; do
  [ -s "$root/$file" ] || fail "missing $file"
done
python3 "$root/tools/check-public-documentation.py" \
  "$root" libpkgstate-source libpkgstate-source.h
if command -v clang++ >/dev/null 2>&1; then
  python3 "$root/tools/check-doxygen-contract.py" \
    --root "$root" --include-subdir libpkgstate-source \
    --include-root "$state_include" \
    --include-root "$source_include" \
    --namespace pkgstate --clang "$(command -v clang++)"
fi

python3 "$root/tools/check-man-markdown.py" \
  --root "$root" --project libpkgstate-source --version 3.0.0
python3 "$root/tools/check-html-manifest.py" \
  --root "$root" --project libpkgstate-source
