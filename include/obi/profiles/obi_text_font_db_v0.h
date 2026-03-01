/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_FONT_DB_V0_H
#define OBI_PROFILE_TEXT_FONT_DB_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_FONT_DB_V0 "obi.profile:text.font_db-0"

enum {
    OBI_FONT_DB_CAP_MATCH_BY_NAME      = 1ull << 0,
    OBI_FONT_DB_CAP_MATCH_BY_CODEPOINT = 1ull << 1,
    OBI_FONT_DB_CAP_SOURCE_BYTES       = 1ull << 2,
    OBI_FONT_DB_CAP_SOURCE_FILE_PATH   = 1ull << 3,
};

typedef enum obi_font_slant_v0 {
    OBI_FONT_SLANT_NORMAL = 0,
    OBI_FONT_SLANT_ITALIC = 1,
    OBI_FONT_SLANT_OBLIQUE = 2,
} obi_font_slant_v0;

typedef struct obi_font_match_req_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional family name (UTF-8, NUL-terminated). */
    const char* family;

    /* Optional style hints. 0 means provider default. */
    uint16_t weight; /* CSS-style 100..900 */
    uint8_t slant;   /* obi_font_slant_v0 */
    uint8_t monospace;

    /* Optional: request a font that supports this Unicode codepoint (0 means ignored). */
    uint32_t codepoint;

    /* Optional BCP47 language tag (UTF-8, NUL-terminated). */
    const char* language;
} obi_font_match_req_v0;

typedef enum obi_font_source_kind_v0 {
    OBI_FONT_SOURCE_NONE      = 0,
    OBI_FONT_SOURCE_BYTES     = 1,
    OBI_FONT_SOURCE_FILE_PATH = 2,
} obi_font_source_kind_v0;

typedef struct obi_font_source_v0 {
    obi_font_source_kind_v0 kind;
    uint32_t face_index; /* TTC/OTC index when applicable; 0 otherwise. */

    /* Provider-owned font source. Valid until release() is called. */
    union {
        obi_bytes_view_v0 bytes;
        obi_utf8_view_v0 file_path; /* UTF-8 path, size excludes any trailing NUL if present */
    } u;

    /* Optional descriptive fields. Provider-owned, valid until release(). */
    obi_utf8_view_v0 resolved_family;
    obi_utf8_view_v0 resolved_style;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_font_source_v0* src);
} obi_font_source_v0;

typedef struct obi_text_font_db_v0 obi_text_font_db_v0;

typedef struct obi_text_font_db_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Match a font face and return a source suitable for loading in text.raster_cache. */
    obi_status (*match_face)(void* ctx,
                             const obi_font_match_req_v0* req,
                             obi_font_source_v0* out_source);
} obi_text_font_db_api_v0;

struct obi_text_font_db_v0 {
    const obi_text_font_db_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_FONT_DB_V0_H */

