/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_MARKDOWN_COMMONMARK_V0_H
#define OBI_PROFILE_DOC_MARKDOWN_COMMONMARK_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_MARKDOWN_COMMONMARK_V0 "obi.profile:doc.markdown_commonmark-0"

enum {
    OBI_MD_CAP_OPTIONS_JSON   = 1ull << 0,
    OBI_MD_CAP_RENDER_HTML    = 1ull << 1,
};

typedef struct obi_md_parse_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_md_parse_params_v0;

typedef struct obi_doc_markdown_commonmark_v0 obi_doc_markdown_commonmark_v0;

typedef struct obi_doc_markdown_commonmark_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Parse markdown into provider-defined JSON (UTF-8) written to out_json. */
    obi_status (*parse_to_json_writer)(void* ctx,
                                       obi_utf8_view_v0 markdown,
                                       const obi_md_parse_params_v0* params,
                                       obi_writer_v0 out_json,
                                       uint64_t* out_bytes_written);

    /* Optional: render markdown to HTML written to out_html. */
    obi_status (*render_to_html_writer)(void* ctx,
                                        obi_utf8_view_v0 markdown,
                                        const obi_md_parse_params_v0* params,
                                        obi_writer_v0 out_html,
                                        uint64_t* out_bytes_written);
} obi_doc_markdown_commonmark_api_v0;

struct obi_doc_markdown_commonmark_v0 {
    const obi_doc_markdown_commonmark_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_MARKDOWN_COMMONMARK_V0_H */

