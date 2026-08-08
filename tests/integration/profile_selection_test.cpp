// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/record_query.h"
#include "../support/test.h"

#include <string>

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("x86_64"),
          pkgsource::architecture_reference("x86_64"));

  TEST_EQ(record.selected_profiles().size(), std::size_t{2});
  for (std::string_view name : {std::string_view("@compiler"),
                                std::string_view("@toolchain")})
  {
    const pkgstate::selected_profile* selected =
        test_support::find_profile(record, name);
    TEST(selected != nullptr);

    const pkgsource::selected_profile* source_selected = nullptr;
    for (const pkgsource::selected_profile& candidate :
         source.recipe().selected_build_profiles())
      if (candidate.profile().name() == name)
        source_selected = &candidate;
    TEST(source_selected != nullptr);

    TEST_EQ(selected->identity().string(),
            std::string("v1:sha256:") + source_selected->identity().hex());
    TEST_EQ(selected->declarations().size(),
            source_selected->declarations().size());
    TEST_EQ(selected->declarations()[0].document(),
            source_selected->declarations()[0].document());
    TEST_EQ(selected->declarations()[0].path(),
            source_selected->declarations()[0].path());
    TEST_EQ(selected->declarations()[0].line(),
            source_selected->declarations()[0].line());
    TEST_EQ(selected->declarations()[0].column(),
            source_selected->declarations()[0].column());
  }
}
