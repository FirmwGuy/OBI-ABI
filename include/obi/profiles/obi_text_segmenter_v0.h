/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_SEGMENTER_V0_H
#define OBI_PROFILE_TEXT_SEGMENTER_V0_H

#include "../obi_core_v0.h"
#include "obi_text_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_SEGMENTER_V0 "obi.profile:text.segmenter-0"

enum {
    OBI_TEXT_SEG_CAP_GRAPHEME  = 1ull << 0,
    OBI_TEXT_SEG_CAP_WORD      = 1ull << 1,
    OBI_TEXT_SEG_CAP_SENTENCE  = 1ull << 2,
    OBI_TEXT_SEG_CAP_LINE      = 1ull << 3,
    OBI_TEXT_SEG_CAP_BIDI      = 1ull << 4,
};

typedef enum obi_text_break_kind_v0 {
    OBI_TEXT_BREAK_ALLOW = 1,
    OBI_TEXT_BREAK_MUST  = 2,
} obi_text_break_kind_v0;

typedef struct obi_text_break_v0 {
    uint32_t byte_offset;
    uint8_t kind; /* obi_text_break_kind_v0 */
    uint8_t reserved[3];
} obi_text_break_v0;

typedef struct obi_text_segmenter_v0 obi_text_segmenter_v0;

typedef struct obi_text_segmenter_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*grapheme_boundaries_utf8)(void* ctx,
                                           obi_utf8_view_v0 text,
                                           obi_text_break_v0* breaks,
                                           size_t break_cap,
                                           size_t* out_break_count);

    obi_status (*word_boundaries_utf8)(void* ctx,
                                       obi_utf8_view_v0 text,
                                       obi_text_break_v0* breaks,
                                       size_t break_cap,
                                       size_t* out_break_count);

    obi_status (*sentence_boundaries_utf8)(void* ctx,
                                           obi_utf8_view_v0 text,
                                           obi_text_break_v0* breaks,
                                           size_t break_cap,
                                           size_t* out_break_count);

    /* Line break opportunities. Providers may return MUST breaks for explicit newlines. */
    obi_status (*line_breaks_utf8)(void* ctx,
                                   obi_utf8_view_v0 text,
                                   obi_text_break_v0* breaks,
                                   size_t break_cap,
                                   size_t* out_break_count);

    /* Optional bidi paragraph segmentation. Output is a list of UTF-8 byte ranges + direction. */
    obi_status (*bidi_paragraph_runs_utf8)(void* ctx,
                                           obi_utf8_view_v0 text,
                                           obi_text_direction_v0 base_dir_hint,
                                           obi_text_bidi_run_v0* runs,
                                           size_t run_cap,
                                           size_t* out_run_count,
                                           obi_text_direction_v0* out_resolved_base_dir);
} obi_text_segmenter_api_v0;

struct obi_text_segmenter_v0 {
    const obi_text_segmenter_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_SEGMENTER_V0_H */

