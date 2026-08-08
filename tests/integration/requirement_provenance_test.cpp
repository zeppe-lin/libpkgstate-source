// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/record_query.h"
#include "../support/test.h"

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("x86_64"),
          pkgsource::architecture_reference("x86_64"));

  TEST_EQ(record.runtime_requirements().size(), std::size_t{3});
  const pkgstate::package_requirement* libfoo =
      test_support::find_runtime(record, "libfoo");
  const pkgstate::package_requirement* libbar =
      test_support::find_runtime(record, "libbar");
  const pkgstate::package_requirement* zlib =
      test_support::find_runtime(record, "zlib");
  TEST(libfoo != nullptr);
  TEST(libbar != nullptr);
  TEST(zlib != nullptr);

  TEST_EQ(libfoo->origins().size(), std::size_t{2});
  bool saw_direct = false;
  bool saw_profile = false;
  for (const pkgstate::requirement_origin& origin : libfoo->origins())
  {
    if (origin.expansion().empty())
    {
      saw_direct = true;
      TEST_EQ(origin.declaration().document(), "recipe.yml");
      TEST_EQ(origin.declaration().path(), "requirements.run[0]");
      TEST_EQ(origin.declaration().line(), std::uint32_t{30});
      TEST_EQ(origin.declaration().column(), std::uint32_t{3});
      continue;
    }

    saw_profile = true;
    TEST_EQ(origin.declaration().path(), "requirements.run[1]");
    TEST_EQ(origin.expansion().size(), std::size_t{2});
    const auto& outer = origin.expansion()[0];
    TEST_EQ(outer.profile().name(), "@runtime");
    TEST_EQ(outer.member_kind(), pkgstate::requirement_member_kind::profile);
    TEST_EQ(outer.member(), "@runtime-core");
    TEST_EQ(outer.provenance().path(), "runtime[0]");
    const auto& leaf = origin.expansion()[1];
    TEST_EQ(leaf.profile().name(), "@runtime-core");
    TEST_EQ(leaf.member_kind(), pkgstate::requirement_member_kind::package);
    TEST_EQ(leaf.member(), "libfoo");
    TEST_EQ(leaf.provenance().path(), "runtime-core[0]");
  }
  TEST(saw_direct);
  TEST(saw_profile);

  TEST_EQ(libbar->origins().size(), std::size_t{1});
  TEST_EQ(libbar->origins()[0].expansion().size(), std::size_t{1});
  TEST_EQ(libbar->origins()[0].expansion()[0].profile().name(), "@runtime");
  TEST_EQ(libbar->origins()[0].expansion()[0].member_kind(),
          pkgstate::requirement_member_kind::package);
  TEST_EQ(libbar->origins()[0].expansion()[0].member(), "libbar");

  TEST_EQ(zlib->origins().size(), std::size_t{1});
  TEST_EQ(zlib->origins()[0].expansion().size(), std::size_t{2});
  TEST_EQ(zlib->origins()[0].expansion().back().member(), "zlib");
}
