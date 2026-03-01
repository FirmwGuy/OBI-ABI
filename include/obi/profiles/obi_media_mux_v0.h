/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_MUX_V0_H
#define OBI_PROFILE_MEDIA_MUX_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_MUX_V0 "obi.profile:media.mux-0"

typedef enum obi_mux_stream_kind_v0 {
    OBI_MUX_STREAM_AUDIO = 0,
    OBI_MUX_STREAM_VIDEO = 1,
    OBI_MUX_STREAM_OTHER = 2,
} obi_mux_stream_kind_v0;

enum {
    OBI_MUX_CAP_OPTIONS_JSON = 1ull << 0,
};

typedef struct obi_mux_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "mp4", "mkv". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_mux_open_params_v0;

typedef struct obi_mux_stream_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_mux_stream_kind_v0 kind;
    uint32_t reserved;

    /* UTF-8 codec ID string, NUL-terminated (e.g. "h264", "aac"). */
    const char* codec_id;

    /* Optional codec extradata bytes (borrowed for duration of add_stream). */
    obi_bytes_view_v0 extradata;

    union {
        struct {
            uint32_t sample_rate_hz;
            uint16_t channels;
            uint16_t reserved16;
        } audio;
        struct {
            uint32_t width;
            uint32_t height;
            uint32_t reserved2;
        } video;
    } u;
} obi_mux_stream_params_v0;

enum {
    OBI_MUX_PACKET_FLAG_DEFAULT  = 0u,
    OBI_MUX_PACKET_FLAG_KEYFRAME = 1u << 0,
};

typedef struct obi_mux_packet_v0 {
    uint32_t stream_index;
    uint32_t flags;

    int64_t pts_ns;
    int64_t dts_ns;
    int64_t duration_ns;

    const void* data;
    size_t size;
} obi_mux_packet_v0;

typedef struct obi_muxer_v0 obi_muxer_v0;

typedef struct obi_muxer_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*add_stream)(void* ctx, const obi_mux_stream_params_v0* params, uint32_t* out_stream_index);

    obi_status (*write_packet)(void* ctx, const obi_mux_packet_v0* pkt);

    /* Finish writing the container (optional; destroy should also finalize). */
    obi_status (*finish)(void* ctx);

    void (*destroy)(void* ctx);
} obi_muxer_api_v0;

struct obi_muxer_v0 {
    const obi_muxer_api_v0* api;
    void* ctx;
};

typedef struct obi_media_mux_v0 obi_media_mux_v0;

typedef struct obi_media_mux_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_writer)(void* ctx,
                              obi_writer_v0 writer,
                              const obi_mux_open_params_v0* params,
                              obi_muxer_v0* out_muxer);
} obi_media_mux_api_v0;

struct obi_media_mux_v0 {
    const obi_media_mux_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_MUX_V0_H */

