/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_VIDEO_SCALE_CONVERT_V0_H
#define OBI_PROFILE_MEDIA_VIDEO_SCALE_CONVERT_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_VIDEO_SCALE_CONVERT_V0 "obi.profile:media.video_scale_convert-0"

enum {
    OBI_VIDEO_SCALE_CONVERT_CAP_RGBA8        = 1ull << 0,
    OBI_VIDEO_SCALE_CONVERT_CAP_BGRA8        = 1ull << 1,
    OBI_VIDEO_SCALE_CONVERT_CAP_RGB8         = 1ull << 2,
    OBI_VIDEO_SCALE_CONVERT_CAP_A8           = 1ull << 3,
    OBI_VIDEO_SCALE_CONVERT_CAP_I420         = 1ull << 4,
    OBI_VIDEO_SCALE_CONVERT_CAP_NV12         = 1ull << 5,

    /* Provider supports provider-specific options_json. */
    OBI_VIDEO_SCALE_CONVERT_CAP_OPTIONS_JSON = 1ull << 16,
};

typedef enum obi_video_scale_filter_v0 {
    OBI_VIDEO_SCALE_FILTER_NEAREST  = 0,
    OBI_VIDEO_SCALE_FILTER_BILINEAR = 1,
    OBI_VIDEO_SCALE_FILTER_BICUBIC  = 2,
} obi_video_scale_filter_v0;

typedef struct obi_video_scale_convert_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_video_scale_filter_v0 filter;
    uint32_t reserved;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_video_scale_convert_params_v0;

typedef struct obi_video_scaler_v0 obi_video_scaler_v0;

typedef struct obi_video_scaler_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Convert/scale a frame from src into dst.
     *
     * - src and dst MUST match the formats used at scaler creation (including width/height).
     * - For packed formats, only planes[0] is used (stride_bytes is bytes per row).
     * - For planar formats, planes[] must be populated according to the pixel format.
     */
    obi_status (*convert)(void* ctx,
                          const obi_video_buffer_view_v0* src,
                          obi_video_buffer_mut_v0* dst);

    void (*destroy)(void* ctx);
} obi_video_scaler_api_v0;

struct obi_video_scaler_v0 {
    const obi_video_scaler_api_v0* api;
    void* ctx;
};

typedef struct obi_media_video_scale_convert_v0 obi_media_video_scale_convert_v0;

typedef struct obi_media_video_scale_convert_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*create_scaler)(void* ctx,
                                obi_video_format_v0 in_fmt,
                                obi_video_format_v0 out_fmt,
                                const obi_video_scale_convert_params_v0* params,
                                obi_video_scaler_v0* out_scaler);
} obi_media_video_scale_convert_api_v0;

struct obi_media_video_scale_convert_v0 {
    const obi_media_video_scale_convert_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_VIDEO_SCALE_CONVERT_V0_H */

