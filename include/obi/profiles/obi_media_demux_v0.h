/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_DEMUX_V0_H
#define OBI_PROFILE_MEDIA_DEMUX_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_DEMUX_V0 "obi.profile:media.demux-0"

typedef enum obi_demux_stream_kind_v0 {
    OBI_DEMUX_STREAM_AUDIO = 0,
    OBI_DEMUX_STREAM_VIDEO = 1,
    OBI_DEMUX_STREAM_OTHER = 2,
} obi_demux_stream_kind_v0;

enum {
    OBI_DEMUX_CAP_OPEN_BYTES     = 1ull << 0,
    OBI_DEMUX_CAP_SEEK           = 1ull << 1,
    OBI_DEMUX_CAP_METADATA_JSON  = 1ull << 2,
};

typedef struct obi_demux_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "mp4", "mkv". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_demux_open_params_v0;

typedef struct obi_demux_stream_info_v0 {
    obi_demux_stream_kind_v0 kind;
    uint32_t reserved;

    /* Provider-owned UTF-8 codec ID string view (e.g. "h264", "aac"). */
    obi_utf8_view_v0 codec_id;

    /* Provider-owned extradata bytes (codec headers); may be empty. */
    obi_bytes_view_v0 extradata;

    /* Optional language tag (UTF-8). */
    obi_utf8_view_v0 language;

    union {
        struct {
            uint32_t sample_rate_hz;
            uint16_t channels;
            uint16_t reserved16;
        } audio;
        struct {
            uint32_t width;
            uint32_t height;
            /* Best-effort pixel format hint when known; may be UNKNOWN. */
            obi_pixel_format_v0 preferred_format;
            uint32_t reserved2;
        } video;
    } u;
} obi_demux_stream_info_v0;

enum {
    OBI_DEMUX_PACKET_FLAG_DEFAULT  = 0u,
    OBI_DEMUX_PACKET_FLAG_KEYFRAME = 1u << 0,
};

typedef struct obi_demux_packet_v0 {
    uint32_t stream_index;
    uint32_t flags;

    /* Best-effort timestamps in nanoseconds; -1 means unknown. */
    int64_t pts_ns;
    int64_t dts_ns;
    int64_t duration_ns;

    /* Provider-owned payload bytes. */
    const void* data;
    size_t size;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_demux_packet_v0* pkt);
} obi_demux_packet_v0;

typedef struct obi_demuxer_v0 obi_demuxer_v0;

typedef struct obi_demuxer_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*stream_count)(void* ctx, uint32_t* out_count);
    obi_status (*stream_info)(void* ctx, uint32_t stream_index, obi_demux_stream_info_v0* out_info);

    /* Read the next packet. Returns OK and sets out_has_packet. */
    obi_status (*read_packet)(void* ctx, obi_demux_packet_v0* out_packet, bool* out_has_packet);

    /* Optional: seek by a container time in nanoseconds. Providers may seek to the nearest keyframe. */
    obi_status (*seek_time_ns)(void* ctx, int64_t time_ns);

    /* Optional container metadata (JSON object string). */
    obi_status (*get_metadata_json)(void* ctx, obi_utf8_view_v0* out_metadata_json);

    void (*destroy)(void* ctx);
} obi_demuxer_api_v0;

struct obi_demuxer_v0 {
    const obi_demuxer_api_v0* api;
    void* ctx;
};

typedef struct obi_media_demux_v0 obi_media_demux_v0;

typedef struct obi_media_demux_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_demux_open_params_v0* params,
                              obi_demuxer_v0* out_demuxer);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_demux_open_params_v0* params,
                             obi_demuxer_v0* out_demuxer);
} obi_media_demux_api_v0;

struct obi_media_demux_v0 {
    const obi_media_demux_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_DEMUX_V0_H */

