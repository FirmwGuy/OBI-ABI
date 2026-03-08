/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_RASTER_CACHE_V0_H
#define OBI_PROFILE_TEXT_RASTER_CACHE_V0_H

#include "../obi_core_v0.h"
#include "obi_text_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_RASTER_CACHE_V0 "obi.profile:text.raster_cache-0"

typedef enum obi_text_bitmap_format_v0 {
    OBI_TEXT_BITMAP_A8    = 0,
    OBI_TEXT_BITMAP_RGBA8 = 1,
} obi_text_bitmap_format_v0;

enum {
    /* Provider can rasterize A8 bitmaps. */
    OBI_TEXT_RASTER_CAP_A8     = 1ull << 0,
    /* Provider can rasterize RGBA8 bitmaps (premultiplied alpha recommended). */
    OBI_TEXT_RASTER_CAP_RGBA8  = 1ull << 1,
    /* Provider supports face_get_glyph_index. */
    OBI_TEXT_RASTER_CAP_CMAP   = 1ull << 2,
};

enum {
    OBI_TEXT_RASTER_FLAG_DEFAULT     = 0u,
    OBI_TEXT_RASTER_FLAG_NO_HINTING  = 1u << 0,
    OBI_TEXT_RASTER_FLAG_MONOCHROME  = 1u << 1,
};

typedef struct obi_text_glyph_bitmap_v0 {
    obi_text_bitmap_format_v0 format;
    uint32_t reserved;

    uint32_t width;
    uint32_t height;
    uint32_t stride_bytes;
    uint32_t reserved2;

    /* Glyph positioning in pixels (compatible with typical y-down renderers):
     * - bitmap_left:  pen_x + bitmap_left is the x coordinate for the bitmap's left edge.
     * - bitmap_top:   pen_y - bitmap_top is the y coordinate for the bitmap's top edge.
     */
    float bitmap_left;
    float bitmap_top;

    /* Advance in pixels for moving the pen after drawing the glyph. */
    float advance_x;
    float advance_y;

    /* Provider-owned pixels. Valid until release() is called (or until the provider is destroyed). */
    const void* pixels;
    size_t pixels_size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_text_glyph_bitmap_v0* bmp);
} obi_text_glyph_bitmap_v0;

typedef struct obi_text_raster_cache_v0 obi_text_raster_cache_v0;

typedef struct obi_text_raster_cache_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create a font face from bytes (TTF/OTF/TTC). face_index selects in collections.
     *
     * This loading contract is intentionally shared with text.shape so hosts can load equivalent
     * provider-local faces into separate shaping/rasterization providers from the same font source.
     */
    obi_status (*face_create_from_bytes)(void* ctx,
                                         obi_bytes_view_v0 font_bytes,
                                         uint32_t face_index,
                                         obi_text_face_id_v0* out_face);

    void (*face_destroy)(void* ctx, obi_text_face_id_v0 face);

    /* Fetch font metrics for a given pixel size. */
    obi_status (*face_get_metrics)(void* ctx,
                                   obi_text_face_id_v0 face,
                                   float px_size,
                                   obi_text_metrics_v0* out_metrics);

    /* Optional: map a Unicode codepoint to a glyph index for the face. */
    obi_status (*face_get_glyph_index)(void* ctx,
                                       obi_text_face_id_v0 face,
                                       uint32_t codepoint,
                                       uint32_t* out_glyph_index);

    /* Rasterize a glyph index at a given pixel size. */
    obi_status (*rasterize_glyph)(void* ctx,
                                  obi_text_face_id_v0 face,
                                  float px_size,
                                  uint32_t glyph_index,
                                  uint32_t flags,
                                  obi_text_glyph_bitmap_v0* out_bitmap);
} obi_text_raster_cache_api_v0;

struct obi_text_raster_cache_v0 {
    const obi_text_raster_cache_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_RASTER_CACHE_V0_H */
