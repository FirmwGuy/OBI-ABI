/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_LAYOUT_V0_H
#define OBI_PROFILE_TEXT_LAYOUT_V0_H

#include "../obi_core_v0.h"
#include "obi_text_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_LAYOUT_V0 "obi.profile:text.layout-0"

enum {
    OBI_TEXT_LAYOUT_CAP_MULTISPAN = 1ull << 0,
    OBI_TEXT_LAYOUT_CAP_WRAP      = 1ull << 1,
    OBI_TEXT_LAYOUT_CAP_ALIGN     = 1ull << 2,
    OBI_TEXT_LAYOUT_CAP_BIDI      = 1ull << 3,
};

typedef enum obi_text_align_v0 {
    OBI_TEXT_ALIGN_LEFT   = 0,
    OBI_TEXT_ALIGN_CENTER = 1,
    OBI_TEXT_ALIGN_RIGHT  = 2,
} obi_text_align_v0;

typedef enum obi_text_wrap_v0 {
    OBI_TEXT_WRAP_NONE = 0,
    OBI_TEXT_WRAP_WORD = 1,
    OBI_TEXT_WRAP_CHAR = 2,
} obi_text_wrap_v0;

typedef struct obi_text_style_span_v0 {
    uint32_t byte_offset;
    uint32_t byte_size;

    obi_text_face_id_v0 face;
    float px_size;
} obi_text_style_span_v0;

typedef struct obi_text_layout_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Layout constraints (0 means unconstrained). */
    float max_width_px;
    float max_height_px;

    /* Optional base direction hint (AUTO/LTR/RTL). */
    obi_text_direction_v0 base_dir;
    uint8_t wrap;  /* obi_text_wrap_v0 */
    uint8_t align; /* obi_text_align_v0 */
    uint8_t reserved8;

    /* Optional line height override (0 means provider default metrics). */
    float line_height_px;
} obi_text_layout_params_v0;

typedef struct obi_text_paragraph_metrics_v0 {
    float width_px;
    float height_px;

    float first_baseline_y;
    float last_baseline_y;

    uint32_t line_count;
    uint32_t glyph_count;
} obi_text_paragraph_metrics_v0;

typedef struct obi_text_line_v0 {
    uint32_t byte_offset;
    uint32_t byte_size;

    uint32_t glyph_start;
    uint32_t glyph_count;

    float baseline_y;
    float ascender;
    float descender;
    float line_height;

    float width_px;
    float reserved;
} obi_text_line_v0;

typedef struct obi_text_positioned_glyph_v0 {
    obi_text_face_id_v0 face;

    uint32_t glyph_index;
    uint32_t cluster; /* byte offset into the original UTF-8 */

    /* Baseline pen position for this glyph (pixels, y-down). */
    float x;
    float y;

    float x_advance;
    float y_advance;
} obi_text_positioned_glyph_v0;

typedef struct obi_text_paragraph_v0 obi_text_paragraph_v0;

typedef struct obi_text_paragraph_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*get_metrics)(void* ctx, obi_text_paragraph_metrics_v0* out_metrics);

    obi_status (*get_lines)(void* ctx,
                            obi_text_line_v0* lines,
                            size_t line_cap,
                            size_t* out_line_count);

    obi_status (*get_glyphs)(void* ctx,
                             obi_text_positioned_glyph_v0* glyphs,
                             size_t glyph_cap,
                             size_t* out_glyph_count);

    void (*destroy)(void* ctx);
} obi_text_paragraph_api_v0;

struct obi_text_paragraph_v0 {
    const obi_text_paragraph_api_v0* api;
    void* ctx;
};

typedef struct obi_text_layout_v0 obi_text_layout_v0;

typedef struct obi_text_layout_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create a paragraph layout.
     *
     * If spans is NULL or span_count is 0, the entire text uses (default_face, default_px_size).
     * If spans are provided, they must be non-overlapping and sorted by byte_offset.
     */
    obi_status (*paragraph_create)(void* ctx,
                                   obi_utf8_view_v0 text,
                                   obi_text_face_id_v0 default_face,
                                   float default_px_size,
                                   const obi_text_layout_params_v0* params,
                                   const obi_text_style_span_v0* spans,
                                   size_t span_count,
                                   obi_text_paragraph_v0* out_paragraph);
} obi_text_layout_api_v0;

struct obi_text_layout_v0 {
    const obi_text_layout_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_LAYOUT_V0_H */

