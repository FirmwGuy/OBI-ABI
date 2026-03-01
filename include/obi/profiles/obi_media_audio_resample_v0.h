/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_AUDIO_RESAMPLE_V0_H
#define OBI_PROFILE_MEDIA_AUDIO_RESAMPLE_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_AUDIO_RESAMPLE_V0 "obi.profile:media.audio_resample-0"

enum {
    OBI_AUDIO_RESAMPLE_CAP_S16          = 1ull << 0,
    OBI_AUDIO_RESAMPLE_CAP_F32          = 1ull << 1,
    /* Provider supports converting channel counts (upmix/downmix) in addition to sample rate. */
    OBI_AUDIO_RESAMPLE_CAP_REMIX        = 1ull << 2,
    /* Provider supports provider-specific options_json. */
    OBI_AUDIO_RESAMPLE_CAP_OPTIONS_JSON = 1ull << 3,
    /* Resampler contexts support reset(). */
    OBI_AUDIO_RESAMPLE_CAP_RESET        = 1ull << 4,
};

typedef struct obi_audio_resample_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional quality knob (0 means provider default). Meaning is provider-defined. */
    uint32_t quality;
    uint32_t reserved;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_audio_resample_params_v0;

typedef struct obi_audio_resampler_v0 obi_audio_resampler_v0;

typedef struct obi_audio_resampler_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Resample input interleaved frames into out_frames.
     *
     * - out_frames is host-owned and must hold at least out_frame_cap frames.
     * - The provider MUST set out_in_frames_consumed and out_out_frames_written (both may be 0).
     */
    obi_status (*process_interleaved)(void* ctx,
                                      const void* in_frames,
                                      size_t in_frame_count,
                                      size_t* out_in_frames_consumed,
                                      void* out_frames,
                                      size_t out_frame_cap,
                                      size_t* out_out_frames_written);

    /* Drain any internally buffered output. Returns OK and sets out_done when fully drained. */
    obi_status (*drain_interleaved)(void* ctx,
                                    void* out_frames,
                                    size_t out_frame_cap,
                                    size_t* out_out_frames_written,
                                    bool* out_done);

    obi_status (*reset)(void* ctx);

    void (*destroy)(void* ctx);
} obi_audio_resampler_api_v0;

struct obi_audio_resampler_v0 {
    const obi_audio_resampler_api_v0* api;
    void* ctx;
};

typedef struct obi_media_audio_resample_v0 obi_media_audio_resample_v0;

typedef struct obi_media_audio_resample_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*create_resampler)(void* ctx,
                                   obi_audio_format_v0 in_fmt,
                                   obi_audio_format_v0 out_fmt,
                                   const obi_audio_resample_params_v0* params,
                                   obi_audio_resampler_v0* out_resampler);
} obi_media_audio_resample_api_v0;

struct obi_media_audio_resample_v0 {
    const obi_media_audio_resample_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_AUDIO_RESAMPLE_V0_H */

