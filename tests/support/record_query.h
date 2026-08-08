// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>

#include <libpkgstate/package_source_record.h>

namespace test_support {

inline const pkgstate::package_requirement*
find_runtime(const pkgstate::package_source_record& record,
             std::string_view package)
{
  for (const pkgstate::package_requirement& requirement :
       record.runtime_requirements())
    if (requirement.package().name() == package)
      return &requirement;
  return nullptr;
}

inline const pkgstate::selected_profile*
find_profile(const pkgstate::package_source_record& record,
             std::string_view profile)
{
  for (const pkgstate::selected_profile& selected : record.selected_profiles())
    if (selected.profile().name() == profile)
      return &selected;
  return nullptr;
}

} // namespace test_support
