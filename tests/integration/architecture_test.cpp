// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/source.h"
#include "../support/test.h"

#include <libpkgstate-source/adapter.h>

namespace {

void restricted_selection_is_exact()
{
  const pkgsource::source_snapshot source = fixture::source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("aarch64"),
          pkgsource::architecture_reference("arm64"));

  TEST_EQ(record.architectures().declared_build().size(), std::size_t{2});
  TEST_EQ(record.architectures().declared_build()[0].name(), "aarch64");
  TEST_EQ(record.architectures().declared_build()[1].name(), "x86_64");
  TEST_EQ(record.architectures().declared_target().size(), std::size_t{2});
  TEST_EQ(record.architectures().declared_target()[0].name(), "arm64");
  TEST_EQ(record.architectures().declared_target()[1].name(), "x86_64");
  TEST_EQ(record.architectures().selected_build().name(), "aarch64");
  TEST_EQ(record.architectures().selected_target().name(), "arm64");
}

void open_selection_is_retained()
{
  const pkgsource::source_snapshot source = fixture::open_architecture_source();
  const pkgstate::package_source_record record =
      pkgstate::source_adapter::project_source(
          source,
          pkgsource::architecture_reference("riscv64"),
          pkgsource::architecture_reference("loongarch64"));
  TEST(record.architectures().declared_build().empty());
  TEST(record.architectures().declared_target().empty());
  TEST_EQ(record.architectures().selected_build().name(), "riscv64");
  TEST_EQ(record.architectures().selected_target().name(), "loongarch64");
}

void rejects_build_selection_only()
{
  const pkgsource::source_snapshot source = fixture::source();
  try
  {
    (void)pkgstate::source_adapter::project_source(
        source,
        pkgsource::architecture_reference("riscv64"),
        pkgsource::architecture_reference("x86_64"));
    TEST(false);
  }
  catch (const pkgstate::source_adapter::projection_error& error)
  {
    TEST_EQ(error.code(),
            pkgstate::source_adapter::projection_error_code::
                architecture_selection);
  }
}

void rejects_target_selection_only()
{
  const pkgsource::source_snapshot source = fixture::source();
  try
  {
    (void)pkgstate::source_adapter::project_source(
        source,
        pkgsource::architecture_reference("x86_64"),
        pkgsource::architecture_reference("riscv64"));
    TEST(false);
  }
  catch (const pkgstate::source_adapter::projection_error& error)
  {
    TEST_EQ(error.code(),
            pkgstate::source_adapter::projection_error_code::
                architecture_selection);
  }
}

} // namespace

int main()
{
  restricted_selection_is_exact();
  open_selection_is_retained();
  rejects_build_selection_only();
  rejects_target_selection_only();
}
