/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_AV_ENCODE_V0_H
#define OBI_PROFILE_MEDIA_AV_ENCODE_V0_H

#include "../obi_core_v0.h"
#include "obi_media_av_decode_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_AV_ENCODE_V0 "obi.profile:media.av_encode-0"

enum {
    OBI_AV_ENC_CAP_AUDIO           = 1ull << 0,
    OBI_AV_ENC_CAP_VIDEO           = 1ull << 1,

    OBI_AV_ENC_CAP_VIDEO_RGBA8     = 1ull << 8,
    OBI_AV_ENC_CAP_AUDIO_S16       = 1ull << 9,
    OBI_AV_ENC_CAP_AUDIO_F32       = 1ull << 10,

    OBI_AV_ENC_CAP_EXTRADATA       = 1ull << 16,
    OBI_AV_ENC_CAP_OPTIONS_JSON    = 1ull << 17,
};

typedef struct obi_av_video_encode_params_v0 {
    obi_video_format_v0 fmt;

    /* Frames per second as a rational. 0/0 means provider default. */
    uint32_t fps_num;
    uint32_t fps_den;

    /* Suggested bitrate; 0 means provider default. */
    uint32_t bitrate_kbps;
    uint32_t reserved;
} obi_av_video_encode_params_v0;

typedef struct obi_av_audio_encode_params_v0 {
    obi_audio_format_v0 fmt;

    /* Suggested bitrate; 0 means provider default. */
    uint32_t bitrate_kbps;
} obi_av_audio_encode_params_v0;

typedef struct obi_av_encode_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    union {
        obi_av_video_encode_params_v0 video;
        obi_av_audio_encode_params_v0 audio;
    } u;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_av_encode_params_v0;

typedef struct obi_video_frame_in_v0 {
    uint32_t width;
    uint32_t height;

    obi_pixel_format_v0 format;
    obi_color_space_v0 color_space;
    obi_alpha_mode_v0 alpha_mode;
    uint32_t reserved;

    uint32_t stride_bytes;
    uint32_t reserved2;

    /* Presentation timestamp in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t duration_ns;

    /* Pixel bytes are borrowed for the duration of the call. */
    const void* pixels;
    size_t pixels_size;
} obi_video_frame_in_v0;

typedef struct obi_audio_frame_in_v0 {
    uint32_t sample_rate_hz;
    uint16_t channels;
    obi_audio_sample_format_v0 format;
    uint8_t reserved8;

    /* Presentation timestamp in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t duration_ns;

    /* Number of audio frames (per-channel samples) in the buffer. */
    uint32_t frame_count;
    uint32_t reserved;

    /* Interleaved PCM samples, borrowed for the duration of the call. */
    const void* samples;
    size_t samples_size;
} obi_audio_frame_in_v0;

typedef struct obi_av_packet_out_v0 {
    const void* data;
    size_t size;

    /* Best-effort timestamps in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t dts_ns;

    uint32_t flags; /* OBI_AV_PACKET_FLAG_* */
    uint32_t reserved;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_av_packet_out_v0* pkt);
} obi_av_packet_out_v0;

typedef struct obi_av_encoder_v0 obi_av_encoder_v0;

typedef struct obi_av_encoder_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Send a raw frame. frame==NULL signals end-of-stream / flush. */
    obi_status (*send_video_frame)(void* ctx, const obi_video_frame_in_v0* frame);
    obi_status (*send_audio_frame)(void* ctx, const obi_audio_frame_in_v0* frame);

    /* Receive an encoded packet. Returns OK and sets out_has_packet. */
    obi_status (*receive_packet)(void* ctx,
                                 obi_av_packet_out_v0* out_packet,
                                 bool* out_has_packet);

    /* Optional: codec extradata (SPS/PPS/etc.) bytes.
     *
     * If out_bytes is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*get_extradata)(void* ctx, void* out_bytes, size_t out_cap, size_t* out_size);

    void (*destroy)(void* ctx);
} obi_av_encoder_api_v0;

struct obi_av_encoder_v0 {
    const obi_av_encoder_api_v0* api;
    void* ctx;
};

typedef struct obi_media_av_encode_v0 obi_media_av_encode_v0;

typedef struct obi_media_av_encode_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*encoder_create)(void* ctx,
                                 obi_av_stream_kind_v0 kind,
                                 const char* codec_id,
                                 const obi_av_encode_params_v0* params,
                                 obi_av_encoder_v0* out_encoder);
} obi_media_av_encode_api_v0;

struct obi_media_av_encode_v0 {
    const obi_media_av_encode_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_AV_ENCODE_V0_H */

