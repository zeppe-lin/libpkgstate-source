// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/test.h"

#include <string>

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::source();
  const pkgstate::package_source_record first =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("aarch64"),
          pkgsource::architecture_reference("arm64"));
  const pkgstate::package_source_record second =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("aarch64"),
          pkgsource::architecture_reference("arm64"));

  TEST(first == second);
  TEST_EQ(first.identity(), second.identity());
  TEST_EQ(first.release().identity().string(),
          std::string("v1:sha256:") +
              source.recipe().release().identity().hex());
  TEST_EQ(first.release().package().name(), "example");
  TEST_EQ(first.release().version(), "1.2.3");
  TEST_EQ(first.release().release(), std::uint32_t{2});

  TEST_EQ(first.metadata().summary(), "Example package");
  TEST(first.metadata().description().has_value());
  TEST_EQ(*first.metadata().description(), "Long description\nwith detail");
  TEST(first.metadata().homepage().has_value());
  TEST_EQ(*first.metadata().homepage(), "https://example.invalid");
  TEST_EQ(first.metadata().licenses().size(), std::size_t{2});
  TEST_EQ(first.metadata().licenses()[0], "MIT");
  TEST_EQ(first.metadata().licenses()[1], "Zlib");

  TEST_EQ(first.snapshot().string(),
          std::string("v1:sha256:") + source.identity().hex());
}
