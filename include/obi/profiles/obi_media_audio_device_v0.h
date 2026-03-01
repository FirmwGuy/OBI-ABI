/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_AUDIO_DEVICE_V0_H
#define OBI_PROFILE_MEDIA_AUDIO_DEVICE_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_AUDIO_DEVICE_V0 "obi.profile:media.audio_device-0"

enum {
    OBI_AUDIO_CAP_OUTPUT            = 1ull << 0,
    OBI_AUDIO_CAP_INPUT             = 1ull << 1,
    OBI_AUDIO_CAP_SAMPLE_S16        = 1ull << 2,
    OBI_AUDIO_CAP_SAMPLE_F32        = 1ull << 3,
    OBI_AUDIO_CAP_LATENCY_QUERY     = 1ull << 4,
};

typedef struct obi_audio_stream_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional device ID/name; NULL means "default device". */
    const char* device_id;

    uint32_t sample_rate_hz;
    uint16_t channels;
    obi_audio_sample_format_v0 format;
    uint8_t reserved8;

    /* Suggested frames per buffer; 0 means provider default. */
    uint32_t buffer_frames;
} obi_audio_stream_params_v0;

typedef struct obi_audio_stream_v0 obi_audio_stream_v0;

typedef struct obi_audio_stream_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Output: write up to frame_count frames. Returns OK and sets out_written (may be 0). */
    obi_status (*write_frames)(void* ctx,
                               const void* frames,
                               size_t frame_count,
                               size_t* out_written);

    /* Input: read up to frame_cap frames into frames. Returns OK and sets out_read (may be 0). */
    obi_status (*read_frames)(void* ctx,
                              void* frames,
                              size_t frame_cap,
                              size_t* out_read);

    /* Optional: query estimated device latency in nanoseconds. */
    obi_status (*get_latency_ns)(void* ctx, uint64_t* out_latency_ns);

    void (*destroy)(void* ctx);
} obi_audio_stream_api_v0;

struct obi_audio_stream_v0 {
    const obi_audio_stream_api_v0* api;
    void* ctx;
};

typedef struct obi_media_audio_device_v0 obi_media_audio_device_v0;

typedef struct obi_media_audio_device_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_output)(void* ctx,
                              const obi_audio_stream_params_v0* params,
                              obi_audio_stream_v0* out_stream);

    obi_status (*open_input)(void* ctx,
                             const obi_audio_stream_params_v0* params,
                             obi_audio_stream_v0* out_stream);
} obi_media_audio_device_api_v0;

struct obi_media_audio_device_v0 {
    const obi_media_audio_device_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_AUDIO_DEVICE_V0_H */

