/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DOC_PAGED_DOCUMENT_V0_H
#define OBI_PROFILE_DOC_PAGED_DOCUMENT_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DOC_PAGED_DOCUMENT_V0 "obi.profile:doc.paged_document-0"

enum {
    OBI_PAGED_CAP_OPEN_BYTES     = 1ull << 0,
    OBI_PAGED_CAP_METADATA_JSON  = 1ull << 1,
    OBI_PAGED_CAP_TEXT_EXTRACT   = 1ull << 2,
};

typedef struct obi_paged_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "pdf", "svg". */
    const char* format_hint;

    /* Optional password (UTF-8, NUL-terminated) for encrypted documents. */
    const char* password_utf8;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_paged_open_params_v0;

typedef struct obi_paged_render_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Render resolution. 0 means provider default (commonly ~96). */
    float dpi;
    float reserved;

    /* Background color used for formats with transparency (e.g. PDF page with alpha). */
    obi_color_rgba8_v0 background;
    uint32_t reserved2;
} obi_paged_render_params_v0;

typedef struct obi_paged_page_image_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format; /* RGBA8 baseline */
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;

    uint32_t stride_bytes;
    uint32_t reserved2;

    const void* pixels;
    size_t pixels_size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_paged_page_image_v0* image);
} obi_paged_page_image_v0;

typedef struct obi_paged_text_v0 {
    /* Provider-owned UTF-8 text view. Valid until release(). */
    obi_utf8_view_v0 text_utf8;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_paged_text_v0* txt);
} obi_paged_text_v0;

typedef struct obi_paged_metadata_v0 {
    /* Provider-owned JSON object string. Valid until release(). */
    obi_utf8_view_v0 metadata_json;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_paged_metadata_v0* meta);
} obi_paged_metadata_v0;

typedef struct obi_paged_document_v0 obi_paged_document_v0;

typedef struct obi_paged_document_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*page_count)(void* ctx, uint32_t* out_page_count);

    /* Page size in points (1/72 inch). */
    obi_status (*page_size_pt)(void* ctx, uint32_t page_index, float* out_w_pt, float* out_h_pt);

    obi_status (*render_page)(void* ctx,
                              uint32_t page_index,
                              const obi_paged_render_params_v0* params,
                              obi_paged_page_image_v0* out_image);

    /* Optional document metadata (JSON). */
    obi_status (*get_metadata_json)(void* ctx, obi_paged_metadata_v0* out_meta);

    /* Optional per-page text extraction. */
    obi_status (*extract_page_text_utf8)(void* ctx, uint32_t page_index, obi_paged_text_v0* out_text);

    void (*destroy)(void* ctx);
} obi_paged_document_api_v0;

struct obi_paged_document_v0 {
    const obi_paged_document_api_v0* api;
    void* ctx;
};

typedef struct obi_doc_paged_document_v0 obi_doc_paged_document_v0;

typedef struct obi_doc_paged_document_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_paged_open_params_v0* params,
                              obi_paged_document_v0* out_doc);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_paged_open_params_v0* params,
                             obi_paged_document_v0* out_doc);
} obi_doc_paged_document_api_v0;

struct obi_doc_paged_document_v0 {
    const obi_doc_paged_document_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DOC_PAGED_DOCUMENT_V0_H */

