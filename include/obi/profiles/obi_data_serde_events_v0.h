/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_SERDE_EVENTS_V0_H
#define OBI_PROFILE_DATA_SERDE_EVENTS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_SERDE_EVENTS_V0 "obi.profile:data.serde_events-0"

enum {
    OBI_SERDE_CAP_OPEN_BYTES    = 1ull << 0,
    OBI_SERDE_CAP_MULTI_DOC     = 1ull << 1,
    OBI_SERDE_CAP_OPTIONS_JSON  = 1ull << 2,
    OBI_SERDE_CAP_LOCATION      = 1ull << 3,
    OBI_SERDE_CAP_LAST_ERROR    = 1ull << 4,
};

typedef struct obi_serde_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "json", "yaml", "toml". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_serde_open_params_v0;

typedef enum obi_serde_event_kind_v0 {
    OBI_SERDE_EVENT_DOC_START   = 0,
    OBI_SERDE_EVENT_DOC_END     = 1,

    OBI_SERDE_EVENT_BEGIN_MAP   = 2,
    OBI_SERDE_EVENT_END_MAP     = 3,
    OBI_SERDE_EVENT_BEGIN_SEQ   = 4,
    OBI_SERDE_EVENT_END_SEQ     = 5,

    OBI_SERDE_EVENT_KEY         = 6,

    OBI_SERDE_EVENT_STRING      = 7,
    OBI_SERDE_EVENT_NUMBER      = 8, /* textual representation */
    OBI_SERDE_EVENT_BOOL        = 9,
    OBI_SERDE_EVENT_NULL        = 10,
} obi_serde_event_kind_v0;

typedef struct obi_serde_event_v0 {
    obi_serde_event_kind_v0 kind;
    uint32_t flags;

    /* Optional location. byte_offset is from start of document; line/column are 1-based.
     * Providers that do not support location MUST set these fields to 0.
     */
    uint64_t byte_offset;
    uint32_t line;
    uint32_t column;
    uint32_t reserved;

    /* For KEY/STRING/NUMBER: provider-owned UTF-8 text view. Valid until the next next_event call or parser destruction. */
    obi_utf8_view_v0 text;

    /* For BOOL: 0 or 1. */
    uint8_t bool_value;
    uint8_t reserved8[7];
} obi_serde_event_v0;

typedef struct obi_serde_parser_v0 obi_serde_parser_v0;

typedef struct obi_serde_parser_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Read the next event. Returns OK and sets out_has_event. */
    obi_status (*next_event)(void* ctx, obi_serde_event_v0* out_event, bool* out_has_event);

    /* Optional: provider-owned error view (UTF-8). Valid until the next next_event call or parser destruction. */
    obi_status (*last_error_utf8)(void* ctx, obi_utf8_view_v0* out_err);

    void (*destroy)(void* ctx);
} obi_serde_parser_api_v0;

struct obi_serde_parser_v0 {
    const obi_serde_parser_api_v0* api;
    void* ctx;
};

typedef struct obi_data_serde_events_v0 obi_data_serde_events_v0;

typedef struct obi_data_serde_events_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_serde_open_params_v0* params,
                              obi_serde_parser_v0* out_parser);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_serde_open_params_v0* params,
                             obi_serde_parser_v0* out_parser);
} obi_data_serde_events_api_v0;

struct obi_data_serde_events_v0 {
    const obi_data_serde_events_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_SERDE_EVENTS_V0_H */

