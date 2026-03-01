/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_MARKUP_EVENTS_V0_H
#define OBI_PROFILE_DOC_MARKUP_EVENTS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_MARKUP_EVENTS_V0 "obi.profile:doc.markup_events-0"

enum {
    OBI_MARKUP_CAP_OPEN_BYTES     = 1ull << 0,
    OBI_MARKUP_CAP_OPTIONS_JSON   = 1ull << 1,
    OBI_MARKUP_CAP_LOCATION       = 1ull << 2,
    OBI_MARKUP_CAP_LAST_ERROR     = 1ull << 3,
};

typedef struct obi_markup_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "xml", "html". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_markup_open_params_v0;

typedef struct obi_markup_attr_kv_v0 {
    /* Provider-owned views. Valid until the next next_event call or parser destruction. */
    obi_utf8_view_v0 key;
    obi_utf8_view_v0 value;
} obi_markup_attr_kv_v0;

typedef enum obi_markup_event_kind_v0 {
    OBI_MARKUP_EVENT_START_ELEMENT = 0,
    OBI_MARKUP_EVENT_END_ELEMENT   = 1,
    OBI_MARKUP_EVENT_TEXT          = 2,
    OBI_MARKUP_EVENT_COMMENT       = 3,
    OBI_MARKUP_EVENT_PI            = 4,
    OBI_MARKUP_EVENT_DOCTYPE       = 5,
    OBI_MARKUP_EVENT_CDATA         = 6,
} obi_markup_event_kind_v0;

enum {
    OBI_MARKUP_EVENT_FLAG_DEFAULT       = 0u,
    /* Start element is an empty-element tag (e.g. <br/>). */
    OBI_MARKUP_EVENT_FLAG_EMPTY_ELEMENT = 1u << 0,
};

typedef struct obi_markup_event_v0 {
    obi_markup_event_kind_v0 kind;
    uint32_t flags;

    /* Optional location. byte_offset is from start of document; line/column are 1-based.
     * Providers that do not support location MUST set these fields to 0.
     */
    uint64_t byte_offset;
    uint32_t line;
    uint32_t column;
    uint32_t reserved;

    union {
        struct {
            obi_utf8_view_v0 name;
            const obi_markup_attr_kv_v0* attrs;
            size_t attr_count;
        } start_element;
        struct {
            obi_utf8_view_v0 name;
        } end_element;
        struct {
            obi_utf8_view_v0 text;
        } text;
        struct {
            obi_utf8_view_v0 text;
        } comment;
        struct {
            obi_utf8_view_v0 target;
            obi_utf8_view_v0 data;
        } pi;
        struct {
            obi_utf8_view_v0 text;
        } doctype;
        struct {
            obi_utf8_view_v0 text;
        } cdata;
    } u;
} obi_markup_event_v0;

typedef struct obi_markup_parser_v0 obi_markup_parser_v0;

typedef struct obi_markup_parser_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Read the next event. Returns OK and sets out_has_event. */
    obi_status (*next_event)(void* ctx, obi_markup_event_v0* out_event, bool* out_has_event);

    /* Optional: provider-owned error view (UTF-8). Valid until the next next_event call or parser destruction. */
    obi_status (*last_error_utf8)(void* ctx, obi_utf8_view_v0* out_err);

    void (*destroy)(void* ctx);
} obi_markup_parser_api_v0;

struct obi_markup_parser_v0 {
    const obi_markup_parser_api_v0* api;
    void* ctx;
};

typedef struct obi_doc_markup_events_v0 obi_doc_markup_events_v0;

typedef struct obi_doc_markup_events_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_markup_open_params_v0* params,
                              obi_markup_parser_v0* out_parser);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_markup_open_params_v0* params,
                             obi_markup_parser_v0* out_parser);
} obi_doc_markup_events_api_v0;

struct obi_doc_markup_events_v0 {
    const obi_doc_markup_events_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_MARKUP_EVENTS_V0_H */

