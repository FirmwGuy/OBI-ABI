/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_AV_DECODE_V0_H
#define OBI_PROFILE_MEDIA_AV_DECODE_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_AV_DECODE_V0 "obi.profile:media.av_decode-0"

typedef enum obi_av_stream_kind_v0 {
    OBI_AV_STREAM_AUDIO = 0,
    OBI_AV_STREAM_VIDEO = 1,
} obi_av_stream_kind_v0;

enum {
    OBI_AV_CAP_AUDIO           = 1ull << 0,
    OBI_AV_CAP_VIDEO           = 1ull << 1,

    OBI_AV_CAP_VIDEO_RGBA8     = 1ull << 8,
    OBI_AV_CAP_AUDIO_S16       = 1ull << 9,
    OBI_AV_CAP_AUDIO_F32       = 1ull << 10,
};

enum {
    OBI_AV_PACKET_FLAG_DEFAULT   = 0u,
    OBI_AV_PACKET_FLAG_KEYFRAME  = 1u << 0,
};

typedef struct obi_av_codec_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional codec extradata (borrowed for the duration of decoder_create). */
    obi_bytes_view_v0 extradata;
} obi_av_codec_params_v0;

typedef struct obi_av_packet_v0 {
    const void* data;
    size_t size;

    /* Best-effort timestamps in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t dts_ns;

    uint32_t flags;
    uint32_t reserved;
} obi_av_packet_v0;

typedef struct obi_video_frame_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format;
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;

    uint32_t stride_bytes;
    uint32_t reserved2;

    /* Best-effort presentation timestamp in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t duration_ns;

    const void* pixels;
    size_t pixels_size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_video_frame_v0* frame);
} obi_video_frame_v0;

typedef struct obi_audio_frame_v0 {
    uint32_t sample_rate_hz;
    uint16_t channels;
    obi_audio_sample_format_v0 format;
    uint8_t reserved8;

    /* Best-effort presentation timestamp in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t duration_ns;

    /* Number of audio frames (per-channel samples) in the buffer. */
    uint32_t frame_count;
    uint32_t reserved;

    const void* samples;
    size_t samples_size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_audio_frame_v0* frame);
} obi_audio_frame_v0;

typedef struct obi_av_decoder_v0 obi_av_decoder_v0;

typedef struct obi_av_decoder_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Send an encoded packet. pkt==NULL signals end-of-stream / flush. */
    obi_status (*send_packet)(void* ctx, const obi_av_packet_v0* pkt);

    /* Receive a decoded video frame. Returns OK and sets out_has_frame. */
    obi_status (*receive_video_frame)(void* ctx,
                                      obi_video_frame_v0* out_frame,
                                      bool* out_has_frame);

    /* Receive a decoded audio frame. Returns OK and sets out_has_frame. */
    obi_status (*receive_audio_frame)(void* ctx,
                                      obi_audio_frame_v0* out_frame,
                                      bool* out_has_frame);

    void (*destroy)(void* ctx);
} obi_av_decoder_api_v0;

struct obi_av_decoder_v0 {
    const obi_av_decoder_api_v0* api;
    void* ctx;
};

typedef struct obi_media_av_decode_v0 obi_media_av_decode_v0;

typedef struct obi_media_av_decode_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*decoder_create)(void* ctx,
                                 obi_av_stream_kind_v0 kind,
                                 const char* codec_id,
                                 const obi_av_codec_params_v0* params,
                                 obi_av_decoder_v0* out_decoder);
} obi_media_av_decode_api_v0;

struct obi_media_av_decode_v0 {
    const obi_media_av_decode_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_AV_DECODE_V0_H */

