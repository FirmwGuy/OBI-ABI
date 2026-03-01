/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_MEDIA_TYPES_V0_H
#define OBI_MEDIA_TYPES_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum obi_color_space_v0 {
    OBI_COLOR_SPACE_UNKNOWN = 0,
    OBI_COLOR_SPACE_SRGB    = 1,
    OBI_COLOR_SPACE_LINEAR  = 2,
} obi_color_space_v0;

typedef enum obi_alpha_mode_v0 {
    OBI_ALPHA_UNKNOWN        = 0,
    OBI_ALPHA_STRAIGHT       = 1,
    OBI_ALPHA_PREMULTIPLIED  = 2,
    OBI_ALPHA_OPAQUE         = 3,
} obi_alpha_mode_v0;

typedef enum obi_pixel_format_v0 {
    OBI_PIXEL_FORMAT_RGBA8 = 0,
    OBI_PIXEL_FORMAT_BGRA8 = 1,
    OBI_PIXEL_FORMAT_RGB8  = 2,
    OBI_PIXEL_FORMAT_A8    = 3,
    /* Planar YUV 4:2:0 (I420/YUV420p). */
    OBI_PIXEL_FORMAT_I420  = 4,
    /* Semi-planar YUV 4:2:0 (NV12). */
    OBI_PIXEL_FORMAT_NV12  = 5,
} obi_pixel_format_v0;

typedef enum obi_audio_sample_format_v0 {
    OBI_AUDIO_SAMPLE_S16 = 0,
    OBI_AUDIO_SAMPLE_F32 = 1,
} obi_audio_sample_format_v0;

typedef struct obi_audio_format_v0 {
    uint32_t sample_rate_hz;
    uint16_t channels;
    obi_audio_sample_format_v0 format;
    uint8_t reserved8;
} obi_audio_format_v0;

#define OBI_MEDIA_MAX_PLANES_V0 3u

typedef struct obi_video_plane_view_v0 {
    const void* data;
    uint32_t stride_bytes;
    uint32_t reserved;
} obi_video_plane_view_v0;

typedef struct obi_video_plane_mut_v0 {
    void* data;
    uint32_t stride_bytes;
    uint32_t reserved;
} obi_video_plane_mut_v0;

typedef struct obi_video_format_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format;
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;
} obi_video_format_v0;

typedef struct obi_video_buffer_view_v0 {
    obi_video_format_v0 fmt;
    obi_video_plane_view_v0 planes[OBI_MEDIA_MAX_PLANES_V0];
} obi_video_buffer_view_v0;

typedef struct obi_video_buffer_mut_v0 {
    obi_video_format_v0 fmt;
    obi_video_plane_mut_v0 planes[OBI_MEDIA_MAX_PLANES_V0];
} obi_video_buffer_mut_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_MEDIA_TYPES_V0_H */
