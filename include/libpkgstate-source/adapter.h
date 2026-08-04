// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*!
 * \file adapter.h
 * \brief Durable projection of sealed libpkgsource authority into state.
 */
#pragma once

#include <libpkgstate-source/export.h>

#include <cstdint>
#include <stdexcept>
#include <string>

#include <libpkgsource/snapshot.h>
#include <libpkgstate/package_source_record.h>

/*! \brief Source-authority admission into the durable state vocabulary. */
namespace pkgstate::source_adapter {

/*! \brief Stable reason that source-to-state projection was refused. */
enum class projection_error_code : std::uint8_t {
  identity_translation = 1,  //!< A foreign identity representation was invalid.
  vocabulary_translation = 2,//!< An owner value had no state representation.
  architecture_selection = 3,//!< A selected architecture was not authorized.
  record_construction = 4,   //!< Native state record invariants rejected input.
};

/*! \brief Typed source-to-state projection failure. */
class PKGSTATE_SOURCE_API projection_error final : public std::invalid_argument {
public:
  /*!
   * \brief Construct a typed projection failure.
   * \param code Stable refusal category.
   * \param message Human-readable diagnostic text.
   */
  projection_error(projection_error_code code, std::string message);

  /*! \brief Destroy the polymorphic projection failure. */
  ~projection_error() override;

  /*!
   * \brief Return the stable refusal category.
  *  \return The stable refusal category.
   */
  [[nodiscard]] projection_error_code code() const noexcept;

private:
  projection_error_code code_;
};

/*!
 * \brief Project one sealed source snapshot into durable state control.
 *
 * The caller supplies the exact build and target architecture selections. The
 * adapter verifies both selections against the sealed recipe and retains the
 * package release, metadata, runtime and lifecycle control, selected build
 * profiles and complete source snapshot identity.
 *
 * The projection performs no syntax parsing, profile selection, dependency
 * resolution, build, application, target observation, or state I/O. YAML or
 * another declaration document is not authority at this boundary; only a
 * sealed source snapshot is accepted.
 *
 * \param source Complete sealed source authority.
 * \param selected_build Exact selected build architecture.
 * \param selected_target Exact selected target architecture.
 * \return Canonical native package source record.
 * \throws projection_error when identity or vocabulary translation fails,
 * architecture selection is not authorized, or state record construction
 * refuses the projected values.
 */
[[nodiscard]] PKGSTATE_SOURCE_API package_source_record project_source(
    const pkgsource::source_snapshot& source,
    const pkgsource::architecture_reference& selected_build,
    const pkgsource::architecture_reference& selected_target);

} // namespace pkgstate::source_adapter
