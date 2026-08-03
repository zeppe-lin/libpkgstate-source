// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include <libpkgstate-source/libpkgstate-source.h>

int
main()
{
  auto* volatile function = &pkgstate::source_adapter::project_source;
  return function == nullptr ? 1 : 0;
}
