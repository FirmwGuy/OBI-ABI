/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_SHAPE_V0_H
#define OBI_PROFILE_TEXT_SHAPE_V0_H

#include "../obi_core_v0.h"
#include "obi_text_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_SHAPE_V0 "obi.profile:text.shape-0"

enum {
    /* Provider supports bidi_paragraph_runs_utf8 for FriBidi-style segmentation. */
    OBI_TEXT_SHAPE_CAP_BIDI_PARAGRAPH = 1ull << 0,
    /* Provider supports OpenType feature strings (for example "kern,liga=0"). */
    OBI_TEXT_SHAPE_CAP_FEATURES       = 1ull << 1,
    /* Provider accepts language tags (BCP47) for shaping decisions. */
    OBI_TEXT_SHAPE_CAP_LANGUAGE       = 1ull << 2,
};

typedef struct obi_text_shape_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_text_direction_v0 direction;
    obi_text_script_tag_v0 script;

    /* Optional, UTF-8, NUL-terminated (BCP47). */
    const char* language;

    /* Optional, UTF-8, NUL-terminated OpenType feature string.
     * Example: "kern,liga=0,ss01=1".
     */
    const char* features;
} obi_text_shape_params_v0;

typedef struct obi_text_shape_v0 obi_text_shape_v0;

typedef struct obi_text_shape_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Shape a UTF-8 text buffer into a sequence of glyphs for a specific face and pixel size.
     *
     * Buffer sizing:
     * - If glyphs is NULL or glyph_cap is 0, the provider MUST set out_glyph_count and return OK.
     * - If glyph_cap is too small, the provider MUST set out_glyph_count and return BUFFER_TOO_SMALL.
     *
     * The provider MUST NOT retain pointers to @text after returning.
     */
    obi_status (*shape_utf8)(void* ctx,
                             obi_text_face_id_v0 face,
                             float px_size,
                             const obi_text_shape_params_v0* params,
                             obi_utf8_view_v0 text,
                             obi_text_glyph_v0* glyphs,
                             size_t glyph_cap,
                             size_t* out_glyph_count,
                             obi_text_direction_v0* out_resolved_direction);

    /* Optional: segment a paragraph into directional runs (bidi analysis). */
    obi_status (*bidi_paragraph_runs_utf8)(void* ctx,
                                           obi_utf8_view_v0 text,
                                           obi_text_direction_v0 base_dir_hint,
                                           obi_text_bidi_run_v0* runs,
                                           size_t run_cap,
                                           size_t* out_run_count,
                                           obi_text_direction_v0* out_resolved_base_dir);
} obi_text_shape_api_v0;

struct obi_text_shape_v0 {
    const obi_text_shape_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_SHAPE_V0_H */

