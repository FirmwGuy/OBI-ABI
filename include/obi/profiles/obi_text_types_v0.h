/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_TYPES_V0_H
#define OBI_PROFILE_TEXT_TYPES_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Provider-local opaque face handle.
 *
 * A face ID is valid only with the provider instance that created it. Separate text providers that
 * load the same font bytes create their own local face IDs; hosts MUST NOT exchange raw face IDs
 * across providers unless a future profile revision explicitly defines such an interop contract.
 */
typedef uint64_t obi_text_face_id_v0;

typedef enum obi_text_direction_v0 {
    OBI_TEXT_DIR_AUTO = 0,
    OBI_TEXT_DIR_LTR  = 1,
    OBI_TEXT_DIR_RTL  = 2,
} obi_text_direction_v0;

/* ISO 15924 script tag packed as big-endian ASCII bytes. Use 0 for unknown. */
typedef uint32_t obi_text_script_tag_v0;

#define OBI_TEXT_SCRIPT_TAG(a, b, c, d) \
    ((obi_text_script_tag_v0)((((uint32_t)(uint8_t)(a)) << 24u) | \
                              (((uint32_t)(uint8_t)(b)) << 16u) | \
                              (((uint32_t)(uint8_t)(c)) << 8u)  | \
                              ((uint32_t)(uint8_t)(d))))

typedef struct obi_text_metrics_v0 {
    /* Ascender/descender are in pixels for the requested size. descender is negative. */
    float ascender;
    float descender;
    float line_gap;
    float line_height; /* recommended baseline-to-baseline distance */
} obi_text_metrics_v0;

typedef struct obi_text_glyph_v0 {
    uint32_t glyph_index; /* glyph index from the loaded font face (0 means "missing"); providers MUST NOT renumber glyphs */
    uint32_t cluster;     /* byte offset into the input UTF-8 */

    /* Positioning in pixels. Advances move the pen; offsets shift the glyph relative to the pen. */
    float x_advance;
    float y_advance;
    float x_offset;
    float y_offset;
} obi_text_glyph_v0;

typedef struct obi_text_bidi_run_v0 {
    uint32_t byte_offset;
    uint32_t byte_size;
    obi_text_direction_v0 dir;
    uint32_t reserved;
} obi_text_bidi_run_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_TYPES_V0_H */
