# Changelog
## OBI-ABI Canonical C Headers

**Document Type:** Changelog
**Status:** Draft
**Last Updated:** 2026-03-08

---

This file tracks notable changes to the canonical OBI C ABI headers in this repository.

The prose specification lives in the sibling `OBI` repository.

## [Unreleased]

### Added

- Added `include/obi/obi_legal_v0.h` with typed legal metadata, dependency closure, route metadata,
  preset policy, and legal-plan result shapes for runtime selector implementations.
- Added `OBI_TEXT_SHAPE_CAP_FACE_CREATE_BYTES` to advertise optional face loading in
  `obi.profile:text.shape-0`.
- Added optional `face_create_from_bytes(...)` and `face_destroy(...)` entrypoints to
  `obi_text_shape_api_v0` so shaping providers can load provider-local faces from the same
  `font bytes + face_index` contract already used by `text.raster_cache-0`.

### Changed

- Extended `obi_provider_api_v0` with the optional `describe_legal_metadata(...)` callback and
  bumped `OBI_CORE_ABI_MINOR` to `3` so hosts can query typed legal facts without relying only on
  ad hoc JSON parsing.
- Clarified in `obi_gfx_types_v0.h` that `obi_window_id_v0` is a provider-instance-local opaque
  handle by default.
- Clarified in `obi_gfx_gpu_device_v0.h` and `obi_gfx_render3d_v0.h` that `begin_frame` /
  `end_frame` window IDs must come from `gfx.window_input-0` on the same provider instance unless a
  provider explicitly documents a bridge.
- Clarified in `obi_text_types_v0.h` that `obi_text_face_id_v0` is provider-local and not portable
  across unrelated text providers.
- Clarified in `obi_text_font_db_v0.h` that the portable split-provider text interop contract is
  `font bytes + face_index`, and that matched font sources are intended for both text rasterization
  and shaping consumers.
- Clarified in `obi_text_raster_cache_v0.h` that face loading from bytes is intentionally shared
  with `text.shape-0` for split-provider workflows.
- Clarified in `obi_text_shape_v0.h` and `obi_text_types_v0.h` that shaped `glyph_index` values are
  the loaded font face's glyph numbering and must not be renumbered into provider-private IDs.
