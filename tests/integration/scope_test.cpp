// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/record_query.h"
#include "../support/test.h"

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::source();
  TEST_EQ(source.recipe().build_requirements().size(), std::size_t{3});
  TEST_EQ(source.recipe().check_requirements().size(), std::size_t{1});

  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("x86_64"),
          pkgsource::architecture_reference("x86_64"));

  TEST(test_support::find_runtime(record, "make") == nullptr);
  TEST(test_support::find_runtime(record, "gcc") == nullptr);
  TEST(test_support::find_runtime(record, "binutils") == nullptr);
  TEST(test_support::find_runtime(record, "tester") == nullptr);
  TEST(test_support::find_runtime(record, "libfoo") != nullptr);
  TEST(test_support::find_runtime(record, "libbar") != nullptr);
  TEST(test_support::find_runtime(record, "zlib") != nullptr);

  TEST_EQ(record.selected_profiles().size(), std::size_t{2});
  TEST_EQ(record.lifecycle_requirements().size(), std::size_t{4});
}
