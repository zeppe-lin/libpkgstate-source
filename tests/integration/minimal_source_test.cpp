// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/test.h"

#include <string>

#include <libpkgstate-source/adapter.h>

int main()
{
  const pkgsource::source_snapshot source = fixture::minimal_source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("riscv64"),
          pkgsource::architecture_reference("loongarch64"));

  TEST_EQ(record.release().package().name(), "minimal");
  TEST_EQ(record.release().version(), "1");
  TEST_EQ(record.release().release(), std::uint32_t{1});
  TEST_EQ(record.metadata().summary(), "Minimal package");
  TEST(!record.metadata().description().has_value());
  TEST(!record.metadata().homepage().has_value());
  TEST_EQ(record.metadata().licenses().size(), std::size_t{1});
  TEST(record.runtime_requirements().empty());
  TEST(record.lifecycle_programs().empty());
  TEST(record.lifecycle_requirements().empty());
  TEST(record.selected_profiles().empty());
  TEST(record.architectures().declared_build().empty());
  TEST(record.architectures().declared_target().empty());
  TEST_EQ(record.architectures().selected_build().name(), "riscv64");
  TEST_EQ(record.architectures().selected_target().name(), "loongarch64");
  TEST_EQ(record.snapshot().string(),
          std::string("v1:sha256:") + source.identity().hex());
}
