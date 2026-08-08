// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/test.h"

#include <array>
#include <string_view>

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("x86_64"),
          pkgsource::architecture_reference("x86_64"));

  struct expected_program final {
    pkgstate::lifecycle_action action;
    std::string_view material;
    std::string_view requirement;
  };
  const std::array<expected_program, 4> expected{{
      {pkgstate::lifecycle_action::pre_install,
       "printf 'pre-install\\n'\n", "preflight"},
      {pkgstate::lifecycle_action::post_install,
       "printf 'post-install\\n'\n", "cache"},
      {pkgstate::lifecycle_action::pre_remove,
       "printf 'pre-remove\\n'\n", "cleanup"},
      {pkgstate::lifecycle_action::post_remove,
       "printf 'post-remove\\n'\n", "finalizer"},
  }};

  TEST_EQ(record.lifecycle_programs().size(), expected.size());
  TEST_EQ(record.lifecycle_requirements().size(), expected.size());
  for (const expected_program& item : expected)
  {
    const pkgstate::lifecycle_program* program = record.lifecycle(item.action);
    TEST(program != nullptr);
    TEST_EQ(program->value().language(), pkgstate::program_language::posix_shell);
    TEST_EQ(program->value().material(), item.material);

    const std::vector<pkgstate::package_requirement> requirements =
        record.lifecycle_requirements(item.action);
    TEST_EQ(requirements.size(), std::size_t{1});
    TEST_EQ(requirements[0].package().name(), item.requirement);
    TEST_EQ(requirements[0].origins().size(), std::size_t{1});
    TEST(requirements[0].origins()[0].expansion().empty());
    TEST_EQ(requirements[0].origins()[0].declaration().document(),
            "recipe.yml");
  }
}
