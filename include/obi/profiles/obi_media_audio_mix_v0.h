/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MEDIA_AUDIO_MIX_V0_H
#define OBI_PROFILE_MEDIA_AUDIO_MIX_V0_H

#include "../obi_core_v0.h"
#include "obi_media_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MEDIA_AUDIO_MIX_V0 "obi.profile:media.audio_mix-0"

enum {
    OBI_AUDIO_MIX_CAP_S16 = 1ull << 0,
    OBI_AUDIO_MIX_CAP_F32 = 1ull << 1,
};

typedef struct obi_audio_mix_format_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint32_t sample_rate_hz;
    uint16_t channels;
    obi_audio_sample_format_v0 format;
    uint8_t reserved8;
} obi_audio_mix_format_v0;

typedef struct obi_audio_mix_input_v0 {
    const void* frames;      /* interleaved PCM frames */
    uint32_t frame_count;    /* frames available in this input */
    float gain;              /* linear gain */
    uint32_t reserved;
} obi_audio_mix_input_v0;

typedef struct obi_media_audio_mix_v0 obi_media_audio_mix_v0;

typedef struct obi_media_audio_mix_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Mix inputs into out_frames.
     *
     * - out_frames is host-owned and must hold at least out_frame_cap frames.
     * - Providers SHOULD mix up to the minimum frame_count among inputs (and out_frame_cap).
     */
    obi_status (*mix_interleaved)(void* ctx,
                                  const obi_audio_mix_format_v0* fmt,
                                  const obi_audio_mix_input_v0* inputs,
                                  size_t input_count,
                                  void* out_frames,
                                  size_t out_frame_cap,
                                  size_t* out_frames_written);
} obi_media_audio_mix_api_v0;

struct obi_media_audio_mix_v0 {
    const obi_media_audio_mix_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MEDIA_AUDIO_MIX_V0_H */
