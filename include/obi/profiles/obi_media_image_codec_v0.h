/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_IMAGE_CODEC_V0_H
#define OBI_PROFILE_MEDIA_IMAGE_CODEC_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_IMAGE_CODEC_V0 "obi.profile:media.image_codec-0"

enum {
    /* Provider supports decode_from_reader. */
    OBI_IMAGE_CAP_DECODE_READER = 1ull << 0,
    /* Provider supports encode_to_writer. */
    OBI_IMAGE_CAP_ENCODE_WRITER = 1ull << 1,
};

typedef struct obi_image_decode_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint, UTF-8 NUL-terminated (examples: "png", "jpeg", "webp"). */
    const char* format_hint;

    /* Preferred output format; provider may fall back if unsupported. */
    obi_pixel_format_v0 preferred_format;
    uint32_t reserved;
} obi_image_decode_params_v0;

typedef struct obi_image_encode_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional quality knob (0 means provider default). Meaning is codec-defined. */
    uint32_t quality;
    uint32_t reserved;
} obi_image_encode_params_v0;

typedef struct obi_image_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format;
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;

    uint32_t stride_bytes;
    uint32_t reserved2;

    /* Provider-owned pixel data. */
    const void* pixels;
    size_t pixels_size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_image_v0* image);
} obi_image_v0;

typedef struct obi_image_pixels_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format;
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;

    uint32_t stride_bytes;
    uint32_t reserved2;

    /* Host-owned pixel data (borrowed for duration of the encode call). */
    const void* pixels;
    size_t pixels_size;
} obi_image_pixels_v0;

typedef struct obi_media_image_codec_v0 obi_media_image_codec_v0;

typedef struct obi_media_image_codec_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*decode_from_bytes)(void* ctx,
                                    obi_bytes_view_v0 bytes,
                                    const obi_image_decode_params_v0* params,
                                    obi_image_v0* out_image);

    obi_status (*decode_from_reader)(void* ctx,
                                     obi_reader_v0 reader,
                                     const obi_image_decode_params_v0* params,
                                     obi_image_v0* out_image);

    obi_status (*encode_to_writer)(void* ctx,
                                   const char* codec_id,
                                   const obi_image_encode_params_v0* params,
                                   const obi_image_pixels_v0* image,
                                   obi_writer_v0 writer,
                                   uint64_t* out_bytes_written);
} obi_media_image_codec_api_v0;

struct obi_media_image_codec_v0 {
    const obi_media_image_codec_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_IMAGE_CODEC_V0_H */

