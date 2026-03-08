/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_URI_V0_H
#define OBI_PROFILE_DATA_URI_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_URI_V0 "obi.profile:data.uri-0"

enum {
    OBI_URI_CAP_RESOLVE           = 1ull << 0,
    OBI_URI_CAP_IRI_UTF8          = 1ull << 1,
    OBI_URI_CAP_FORM_URLENCODED   = 1ull << 2,
};

typedef enum obi_uri_component_kind_v0 {
    OBI_URI_COMPONENT_USERINFO    = 0,
    OBI_URI_COMPONENT_PATH_SEGMENT = 1,
    OBI_URI_COMPONENT_QUERY_KEY   = 2,
    OBI_URI_COMPONENT_QUERY_VALUE = 3,
    OBI_URI_COMPONENT_FRAGMENT    = 4,
} obi_uri_component_kind_v0;

enum {
    /* Query parsing input may start with '?'. */
    OBI_URI_QUERY_ALLOW_LEADING_QMARK = 1u << 0,
    /* Treat '+' as space in query key/value contexts. */
    OBI_URI_QUERY_PLUS_AS_SPACE       = 1u << 1,
};

enum {
    /* Emit '+' for spaces when encoding query form values, or decode '+' as space. */
    OBI_URI_PERCENT_SPACE_AS_PLUS     = 1u << 0,
};

typedef struct obi_uri_parts_v0 {
    /* Provider-owned UTF-8 views preserving URI syntax; percent-encoding is not automatically
     * decoded.
     */
    obi_utf8_view_v0 scheme;
    obi_utf8_view_v0 userinfo;
    obi_utf8_view_v0 host;
    obi_utf8_view_v0 path;
    obi_utf8_view_v0 query;
    obi_utf8_view_v0 fragment;

    /* -1 means no explicit port was present. */
    int32_t port;

    uint8_t has_authority;
    uint8_t path_is_absolute;
    uint8_t reserved8[2];
} obi_uri_parts_v0;

typedef struct obi_uri_text_v0 {
    /* Provider-owned UTF-8 view. Valid until release(). */
    obi_utf8_view_v0 text;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_uri_text_v0* out_text);
} obi_uri_text_v0;

typedef struct obi_uri_query_item_v0 {
    /* Provider-owned decoded UTF-8 views. */
    obi_utf8_view_v0 key;
    obi_utf8_view_v0 value;

    /* Distinguishes "flag" from "flag=". */
    uint8_t has_value;
    uint8_t reserved8[7];
} obi_uri_query_item_v0;

typedef struct obi_uri_query_items_v0 {
    const obi_uri_query_item_v0* items;
    size_t count;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_uri_query_items_v0* out_items);
} obi_uri_query_items_v0;

typedef struct obi_uri_info_v0 {
    obi_uri_parts_v0 parts;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_uri_info_v0* out_info);
} obi_uri_info_v0;

typedef struct obi_data_uri_v0 obi_data_uri_v0;

typedef struct obi_data_uri_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Parse a UTF-8 URI reference into raw syntax-preserving components. */
    obi_status (*parse_utf8)(void* ctx,
                             obi_utf8_view_v0 uri,
                             obi_uri_info_v0* out_info);

    /* Normalize a UTF-8 URI reference. Providers SHOULD apply RFC 3986-safe normalization rules. */
    obi_status (*normalize_utf8)(void* ctx,
                                 obi_utf8_view_v0 uri,
                                 uint32_t flags,
                                 obi_uri_text_v0* out_text);

    /* Optional: resolve a relative reference against a base URI. */
    obi_status (*resolve_utf8)(void* ctx,
                               obi_utf8_view_v0 base_uri,
                               obi_utf8_view_v0 ref_uri,
                               uint32_t flags,
                               obi_uri_text_v0* out_text);

    /* Parse a raw query string (without leading '?' unless flags allow it) into ordered key/value
     * pairs.
     */
    obi_status (*query_items_utf8)(void* ctx,
                                   obi_utf8_view_v0 query,
                                   uint32_t flags,
                                   obi_uri_query_items_v0* out_items);

    /* Percent-encode a UTF-8 component. */
    obi_status (*percent_encode_utf8)(void* ctx,
                                      obi_uri_component_kind_v0 component,
                                      obi_utf8_view_v0 text,
                                      uint32_t flags,
                                      obi_uri_text_v0* out_text);

    /* Percent-decode a UTF-8 component. Invalid escapes or non-UTF-8 octet sequences are errors. */
    obi_status (*percent_decode_utf8)(void* ctx,
                                      obi_uri_component_kind_v0 component,
                                      obi_utf8_view_v0 text,
                                      uint32_t flags,
                                      obi_uri_text_v0* out_text);
} obi_data_uri_api_v0;

struct obi_data_uri_v0 {
    const obi_data_uri_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_URI_V0_H */

