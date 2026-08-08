// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <libpkgsource/libpkgsource.h>

namespace fixture {

inline pkgsource::declaration_provenance
at(std::string document, std::string path, std::uint32_t line,
   std::uint32_t column = 3)
{
  return pkgsource::declaration_provenance(
      std::move(document), std::move(path), line, column);
}

inline pkgsource::profile_catalog profiles()
{
  return pkgsource::profile_catalog::seal({
      pkgsource::profile_declaration(
          pkgsource::profile_reference("@compiler"),
          at("profiles.yml", "compiler", 1),
          {pkgsource::profile_member_declaration(
              pkgsource::requirement_subject(
                  pkgsource::package_reference("gcc")),
              at("profiles.yml", "compiler[0]", 2))}),
      pkgsource::profile_declaration(
          pkgsource::profile_reference("@toolchain"),
          at("profiles.yml", "toolchain", 4),
          {
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::package_reference("binutils")),
                  at("profiles.yml", "toolchain[0]", 5)),
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::profile_reference("@compiler")),
                  at("profiles.yml", "toolchain[1]", 6)),
          }),
      pkgsource::profile_declaration(
          pkgsource::profile_reference("@runtime-core"),
          at("profiles.yml", "runtime-core", 10),
          {
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::package_reference("libfoo")),
                  at("profiles.yml", "runtime-core[0]", 11)),
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::package_reference("zlib")),
                  at("profiles.yml", "runtime-core[1]", 12)),
          }),
      pkgsource::profile_declaration(
          pkgsource::profile_reference("@runtime"),
          at("profiles.yml", "runtime", 14),
          {
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::profile_reference("@runtime-core")),
                  at("profiles.yml", "runtime[0]", 15)),
              pkgsource::profile_member_declaration(
                  pkgsource::requirement_subject(
                      pkgsource::package_reference("libbar")),
                  at("profiles.yml", "runtime[1]", 16)),
          }),
  });
}

inline pkgsource::requirement_declaration
package_requirement(pkgsource::requirement_scope scope, std::string package,
                    std::string path, std::uint32_t line)
{
  return pkgsource::requirement_declaration(
      std::move(scope),
      pkgsource::requirement_subject(
          pkgsource::package_reference(std::move(package))),
      at("recipe.yml", std::move(path), line));
}

inline pkgsource::requirement_declaration
profile_requirement(pkgsource::requirement_scope scope, std::string profile,
                    std::string path, std::uint32_t line)
{
  return pkgsource::requirement_declaration(
      std::move(scope),
      pkgsource::requirement_subject(
          pkgsource::profile_reference(std::move(profile))),
      at("recipe.yml", std::move(path), line));
}

inline pkgsource::lifecycle_program
lifecycle(pkgsource::lifecycle_action action, std::string material)
{
  return pkgsource::lifecycle_program(
      action,
      pkgsource::program(pkgsource::program_language::posix_shell,
                         std::move(material)));
}

inline pkgsource::source_snapshot source(
    std::vector<pkgsource::architecture_reference> build_architectures = {
        pkgsource::architecture_reference("x86_64"),
        pkgsource::architecture_reference("aarch64")},
    std::vector<pkgsource::architecture_reference> target_architectures = {
        pkgsource::architecture_reference("x86_64"),
        pkgsource::architecture_reference("arm64")})
{
  const pkgsource::profile_catalog catalog = profiles();
  return pkgsource::seal_source(
      pkgsource::source_origin("recipe.yml"),
      pkgsource::recipe_declaration(
          pkgsource::package_release(
              pkgsource::package_reference("example"), "1.2.3", 2),
          pkgsource::package_metadata(
              "Example package", "Long description\nwith detail",
              "https://example.invalid", {"Zlib", "MIT"}),
          {
              pkgsource::source_input::remote(
                  "https://example.invalid/example.tar.xz", "example.tar.xz",
                  pkgsource::digest(
                      pkgsource::digest_algorithm::sha256,
                      "1111111111111111111111111111111111111111111111111111111111111111")),
          },
          pkgsource::program(
              pkgsource::program_language::posix_shell,
              "printf 'build\\n'\n"),
          {
              profile_requirement(pkgsource::requirement_scope::build(),
                                  "@toolchain", "requirements.build[0]", 20),
              profile_requirement(pkgsource::requirement_scope::build(),
                                  "@compiler", "requirements.build[1]", 21),
              package_requirement(pkgsource::requirement_scope::build(),
                                  "make", "requirements.build[2]", 22),
              package_requirement(pkgsource::requirement_scope::run(),
                                  "libfoo", "requirements.run[0]", 30),
              profile_requirement(pkgsource::requirement_scope::run(),
                                  "@runtime", "requirements.run[1]", 31),
              package_requirement(pkgsource::requirement_scope::check(),
                                  "tester", "requirements.check[0]", 40),
              package_requirement(
                  pkgsource::requirement_scope::lifecycle(
                      pkgsource::lifecycle_action::pre_install),
                  "preflight", "requirements.lifecycle.pre-install[0]", 50),
              package_requirement(
                  pkgsource::requirement_scope::lifecycle(
                      pkgsource::lifecycle_action::post_install),
                  "cache", "requirements.lifecycle.post-install[0]", 51),
              package_requirement(
                  pkgsource::requirement_scope::lifecycle(
                      pkgsource::lifecycle_action::pre_remove),
                  "cleanup", "requirements.lifecycle.pre-remove[0]", 52),
              package_requirement(
                  pkgsource::requirement_scope::lifecycle(
                      pkgsource::lifecycle_action::post_remove),
                  "finalizer", "requirements.lifecycle.post-remove[0]", 53),
          },
          {
              lifecycle(pkgsource::lifecycle_action::pre_install,
                        "printf 'pre-install\\n'\n"),
              lifecycle(pkgsource::lifecycle_action::post_install,
                        "printf 'post-install\\n'\n"),
              lifecycle(pkgsource::lifecycle_action::pre_remove,
                        "printf 'pre-remove\\n'\n"),
              lifecycle(pkgsource::lifecycle_action::post_remove,
                        "printf 'post-remove\\n'\n"),
          },
          pkgsource::architecture_requirements(
              std::move(build_architectures),
              std::move(target_architectures)),
          at("recipe.yml", "$", 1),
          pkgsource::program(
              pkgsource::program_language::posix_shell,
              "printf 'check\\n'\n")),
      catalog);
}

inline pkgsource::source_snapshot minimal_source()
{
  const pkgsource::profile_catalog catalog =
      pkgsource::profile_catalog::seal({});
  return pkgsource::seal_source(
      pkgsource::source_origin("minimal.recipe"),
      pkgsource::recipe_declaration(
          pkgsource::package_release(
              pkgsource::package_reference("minimal"), "1", 1),
          pkgsource::package_metadata(
              "Minimal package", std::nullopt, std::nullopt, {"MIT"}),
          {},
          pkgsource::program(
              pkgsource::program_language::posix_shell, "printf 'build\\n'\n"),
          {}, {}, pkgsource::architecture_requirements({}, {}),
          at("minimal.recipe", "$", 1)),
      catalog);
}

inline pkgsource::source_snapshot open_architecture_source()
{
  return source({}, {});
}

} // namespace fixture
