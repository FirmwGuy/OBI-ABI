/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_REGEX_V0_H
#define OBI_PROFILE_TEXT_REGEX_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_REGEX_V0 "obi.profile:text.regex-0"

enum {
    OBI_REGEX_CAP_NAMED_CAPTURES    = 1ull << 0,
    OBI_REGEX_CAP_UNICODE_PROPERTIES = 1ull << 1,
};

enum {
    OBI_REGEX_COMPILE_CASE_INSENSITIVE = 1u << 0,
    OBI_REGEX_COMPILE_MULTILINE        = 1u << 1,
    OBI_REGEX_COMPILE_DOTALL           = 1u << 2,
    OBI_REGEX_COMPILE_EXTENDED         = 1u << 3,
    OBI_REGEX_COMPILE_ANCHORED         = 1u << 4,
};

enum {
    OBI_REGEX_MATCH_NOTBOL   = 1u << 0,
    OBI_REGEX_MATCH_NOTEOL   = 1u << 1,
    OBI_REGEX_MATCH_ANCHORED = 1u << 2,
};

enum {
    OBI_REGEX_REPLACE_ALL     = 1u << 0,
    OBI_REGEX_REPLACE_LITERAL = 1u << 1,
};

typedef struct obi_regex_capture_span_v0 {
    uint64_t byte_start;
    uint64_t byte_end;

    /* 1 if this capture participated in the match, 0 otherwise. */
    uint8_t matched;
    uint8_t reserved8[7];
} obi_regex_capture_span_v0;

typedef struct obi_regex_v0 obi_regex_v0;

typedef struct obi_regex_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Returns the number of capture groups, excluding the implicit whole-match group. */
    obi_status (*group_count)(void* ctx, uint32_t* out_group_count);

    /* Match against the entire haystack starting at offset 0. If matched, spans[0] is the whole
     * match and subsequent entries are capture groups.
     */
    obi_status (*match_utf8)(void* ctx,
                             obi_utf8_view_v0 haystack,
                             uint32_t flags,
                             obi_regex_capture_span_v0* spans,
                             size_t span_cap,
                             size_t* out_span_count,
                             bool* out_matched);

    /* Search for the next match at or after start_byte_offset. The offset must be at a UTF-8
     * codepoint boundary.
     */
    obi_status (*find_next_utf8)(void* ctx,
                                 obi_utf8_view_v0 haystack,
                                 uint64_t start_byte_offset,
                                 uint32_t flags,
                                 obi_regex_capture_span_v0* spans,
                                 size_t span_cap,
                                 size_t* out_span_count,
                                 bool* out_matched);

    /* Optional: resolve a named capture to its numeric index (1-based, matching span order). */
    obi_status (*capture_name_to_index)(void* ctx,
                                        obi_utf8_view_v0 name,
                                        uint32_t* out_capture_index);

    /* Replace matches in haystack and write UTF-8 output into a caller buffer.
     *
     * Replacement syntax:
     * - $0 => whole match
     * - $1..$99 => numbered captures
     * - $$ => literal '$'
     * - ${name} => named capture (when supported)
     *
     * If out is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size to the
     * required size including the NUL terminator.
     */
    obi_status (*replace_utf8)(void* ctx,
                               obi_utf8_view_v0 haystack,
                               obi_utf8_view_v0 replacement,
                               uint32_t flags,
                               char* out,
                               size_t out_cap,
                               size_t* out_size,
                               uint32_t* out_replacement_count);

    void (*destroy)(void* ctx);
} obi_regex_api_v0;

struct obi_regex_v0 {
    const obi_regex_api_v0* api;
    void* ctx;
};

typedef struct obi_text_regex_v0 obi_text_regex_v0;

typedef struct obi_text_regex_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Compile a UTF-8 pattern into a reusable regex handle. */
    obi_status (*compile_utf8)(void* ctx,
                               obi_utf8_view_v0 pattern,
                               uint32_t compile_flags,
                               obi_regex_v0* out_regex);
} obi_text_regex_api_v0;

struct obi_text_regex_v0 {
    const obi_text_regex_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_REGEX_V0_H */

