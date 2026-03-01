/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_MARKDOWN_EVENTS_V0_H
#define OBI_PROFILE_DOC_MARKDOWN_EVENTS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_MARKDOWN_EVENTS_V0 "obi.profile:doc.markdown_events-0"

enum {
    OBI_MD_EVENTS_CAP_OPTIONS_JSON  = 1ull << 0,
    OBI_MD_EVENTS_CAP_LOCATION      = 1ull << 1,
    OBI_MD_EVENTS_CAP_LAST_ERROR    = 1ull << 2,
};

typedef struct obi_md_events_parse_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_md_events_parse_params_v0;

typedef enum obi_md_event_kind_v0 {
    OBI_MD_EVENT_ENTER = 0,
    OBI_MD_EVENT_EXIT  = 1,
} obi_md_event_kind_v0;

typedef enum obi_md_node_kind_v0 {
    OBI_MD_NODE_DOCUMENT        = 0,

    /* Block nodes */
    OBI_MD_NODE_BLOCK_QUOTE     = 1,
    OBI_MD_NODE_LIST            = 2,
    OBI_MD_NODE_ITEM            = 3,
    OBI_MD_NODE_CODE_BLOCK      = 4,
    OBI_MD_NODE_HTML_BLOCK      = 5,
    OBI_MD_NODE_PARAGRAPH       = 6,
    OBI_MD_NODE_HEADING         = 7,
    OBI_MD_NODE_THEMATIC_BREAK  = 8,

    /* Inline nodes */
    OBI_MD_NODE_TEXT            = 9,
    OBI_MD_NODE_SOFTBREAK       = 10,
    OBI_MD_NODE_LINEBREAK       = 11,
    OBI_MD_NODE_CODE            = 12,
    OBI_MD_NODE_HTML_INLINE     = 13,
    OBI_MD_NODE_EMPH            = 14,
    OBI_MD_NODE_STRONG          = 15,
    OBI_MD_NODE_LINK            = 16,
    OBI_MD_NODE_IMAGE           = 17,
} obi_md_node_kind_v0;

typedef struct obi_md_attr_kv_v0 {
    /* Provider-owned views. Valid until the next next_event call or parser destruction. */
    obi_utf8_view_v0 key;
    obi_utf8_view_v0 value;
} obi_md_attr_kv_v0;

typedef struct obi_md_event_v0 {
    obi_md_event_kind_v0 event_kind;
    obi_md_node_kind_v0 node_kind;
    uint32_t flags;
    uint32_t reserved;

    /* Optional location. byte_offset is from start of document; line/column are 1-based.
     * Providers that do not support location MUST set these fields to 0.
     */
    uint64_t byte_offset;
    uint32_t line;
    uint32_t column;
    uint32_t reserved2;

    /* Optional literal text for leaf nodes (TEXT, CODE, CODE_BLOCK, HTML_*).
     * Provider-owned view; valid until the next next_event call or parser destruction.
     */
    obi_utf8_view_v0 literal;

    /* Optional attribute list for ENTER events (heading level, link URL/title, list kind, etc.). */
    const obi_md_attr_kv_v0* attrs;
    size_t attr_count;
} obi_md_event_v0;

typedef struct obi_md_event_parser_v0 obi_md_event_parser_v0;

typedef struct obi_md_event_parser_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Read the next event. Returns OK and sets out_has_event. */
    obi_status (*next_event)(void* ctx, obi_md_event_v0* out_event, bool* out_has_event);

    /* Optional: provider-owned error view (UTF-8). Valid until the next next_event call or parser destruction. */
    obi_status (*last_error_utf8)(void* ctx, obi_utf8_view_v0* out_err);

    void (*destroy)(void* ctx);
} obi_md_event_parser_api_v0;

struct obi_md_event_parser_v0 {
    const obi_md_event_parser_api_v0* api;
    void* ctx;
};

typedef struct obi_doc_markdown_events_v0 obi_doc_markdown_events_v0;

typedef struct obi_doc_markdown_events_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Parse a UTF-8 markdown buffer into an event parser. */
    obi_status (*parse_utf8)(void* ctx,
                             obi_utf8_view_v0 markdown,
                             const obi_md_events_parse_params_v0* params,
                             obi_md_event_parser_v0* out_parser);
} obi_doc_markdown_events_api_v0;

struct obi_doc_markdown_events_v0 {
    const obi_doc_markdown_events_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_MARKDOWN_EVENTS_V0_H */

