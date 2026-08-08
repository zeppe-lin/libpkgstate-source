// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../support/test.h"

#include <stdexcept>
#include <string>

#include <libpkgstate-source/adapter.h>

int main()
{
  using pkgstate::source_adapter::projection_error;
  using pkgstate::source_adapter::projection_error_code;

  const projection_error error(
      projection_error_code::vocabulary_translation,
      "cannot translate future source vocabulary");
  TEST_EQ(error.code(), projection_error_code::vocabulary_translation);
  TEST_EQ(std::string(error.what()),
          "cannot translate future source vocabulary");

  const std::invalid_argument& base = error;
  TEST_EQ(std::string(base.what()),
          "cannot translate future source vocabulary");
}
