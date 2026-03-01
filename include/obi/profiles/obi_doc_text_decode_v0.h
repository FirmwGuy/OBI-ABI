/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_TEXT_DECODE_V0_H
#define OBI_PROFILE_DOC_TEXT_DECODE_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_TEXT_DECODE_V0 "obi.profile:doc.text_decode-0"

enum {
    OBI_TEXT_DECODE_CAP_FROM_READER   = 1ull << 0,
    OBI_TEXT_DECODE_CAP_CONFIDENCE    = 1ull << 1,
};

enum {
    OBI_TEXT_DECODE_FLAG_DEFAULT        = 0u,
    /* Fail on invalid sequences. */
    OBI_TEXT_DECODE_FLAG_STRICT         = 1u << 0,
    /* Replace invalid sequences with U+FFFD. */
    OBI_TEXT_DECODE_FLAG_REPLACE_INVALID = 1u << 1,
};

typedef struct obi_doc_text_decode_params_v0 {
    uint32_t struct_size;
    uint32_t flags; /* OBI_TEXT_DECODE_FLAG_* */

    /* Optional encoding hint (UTF-8, NUL-terminated). Examples: "utf-8", "windows-1252". */
    const char* encoding_hint;

    /* Maximum probe bytes for detection when encoding_hint is NULL (0 means provider default). */
    uint32_t max_probe_bytes;
    uint32_t reserved;
} obi_doc_text_decode_params_v0;

typedef struct obi_doc_text_decode_info_v0 {
    /* Provider-owned view; valid until release() is called. */
    obi_utf8_view_v0 detected_encoding;

    /* Optional 0..100 confidence; 0 if unknown/unsupported. */
    uint32_t confidence;
    uint32_t had_errors; /* nonzero if invalid sequences were encountered */

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_doc_text_decode_info_v0* info);
} obi_doc_text_decode_info_v0;

typedef struct obi_doc_text_decode_v0 obi_doc_text_decode_v0;

typedef struct obi_doc_text_decode_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*decode_bytes_to_utf8_writer)(void* ctx,
                                              obi_bytes_view_v0 bytes,
                                              const obi_doc_text_decode_params_v0* params,
                                              obi_writer_v0 utf8_out,
                                              obi_doc_text_decode_info_v0* out_info,
                                              uint64_t* out_bytes_in,
                                              uint64_t* out_bytes_out);

    obi_status (*decode_reader_to_utf8_writer)(void* ctx,
                                               obi_reader_v0 reader,
                                               const obi_doc_text_decode_params_v0* params,
                                               obi_writer_v0 utf8_out,
                                               obi_doc_text_decode_info_v0* out_info,
                                               uint64_t* out_bytes_in,
                                               uint64_t* out_bytes_out);
} obi_doc_text_decode_api_v0;

struct obi_doc_text_decode_v0 {
    const obi_doc_text_decode_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_TEXT_DECODE_V0_H */

