/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_INSPECT_V0_H
#define OBI_PROFILE_DOC_INSPECT_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_INSPECT_V0 "obi.profile:doc.inspect-0"

enum {
    OBI_DOC_INSPECT_CAP_FROM_READER   = 1ull << 0,
    OBI_DOC_INSPECT_CAP_SUMMARY_JSON  = 1ull << 1,
    OBI_DOC_INSPECT_CAP_METADATA_JSON = 1ull << 2,
    OBI_DOC_INSPECT_CAP_ENCODING      = 1ull << 3,
    OBI_DOC_INSPECT_CAP_CONFIDENCE    = 1ull << 4,
};

typedef struct obi_doc_inspect_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Maximum bytes the provider may read when probing a reader (0 means provider default). */
    uint32_t max_probe_bytes;
    uint32_t reserved;

    /* If 0, provider may omit JSON fields even if supported. */
    uint8_t want_summary_json;
    uint8_t want_metadata_json;
    uint8_t reserved8[2];
} obi_doc_inspect_params_v0;

typedef struct obi_doc_inspect_info_v0 {
    /* Provider-owned views. Valid until release() is called. */
    obi_utf8_view_v0 mime_type;
    obi_utf8_view_v0 format_id;

    /* Optional provider-owned views. */
    obi_utf8_view_v0 description;
    obi_utf8_view_v0 encoding;      /* only when applicable; may be empty */
    obi_utf8_view_v0 summary_json;  /* JSON object string */
    obi_utf8_view_v0 metadata_json; /* JSON object string */

    /* Optional confidence 0..100; 0 if unknown/unsupported. */
    uint32_t confidence;
    uint32_t reserved;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_doc_inspect_info_v0* info);
} obi_doc_inspect_info_v0;

typedef struct obi_doc_inspect_v0 obi_doc_inspect_v0;

typedef struct obi_doc_inspect_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*inspect_from_bytes)(void* ctx,
                                     obi_bytes_view_v0 bytes,
                                     const obi_doc_inspect_params_v0* params,
                                     obi_doc_inspect_info_v0* out_info);

    obi_status (*inspect_from_reader)(void* ctx,
                                      obi_reader_v0 reader,
                                      const obi_doc_inspect_params_v0* params,
                                      obi_doc_inspect_info_v0* out_info);
} obi_doc_inspect_api_v0;

struct obi_doc_inspect_v0 {
    const obi_doc_inspect_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_INSPECT_V0_H */

