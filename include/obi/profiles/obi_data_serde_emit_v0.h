/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_SERDE_EMIT_V0_H
#define OBI_PROFILE_DATA_SERDE_EMIT_V0_H

#include "../obi_core_v0.h"
#include "obi_data_serde_events_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_SERDE_EMIT_V0 "obi.profile:data.serde_emit-0"

enum {
    OBI_SERDE_EMIT_CAP_MULTI_DOC     = 1ull << 0,
    OBI_SERDE_EMIT_CAP_OPTIONS_JSON  = 1ull << 1,
    OBI_SERDE_EMIT_CAP_LAST_ERROR    = 1ull << 2,
};

typedef struct obi_serde_emit_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Required format hint (UTF-8, NUL-terminated), e.g. "json", "yaml", "toml". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_serde_emit_open_params_v0;

typedef struct obi_serde_emitter_v0 obi_serde_emitter_v0;

typedef struct obi_serde_emitter_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Emit one event. The event set matches data.serde_events. */
    obi_status (*emit)(void* ctx, const obi_serde_event_v0* ev);

    /* Optional: provider-owned error view (UTF-8). Valid until the next emit or destruction. */
    obi_status (*last_error_utf8)(void* ctx, obi_utf8_view_v0* out_err);

    /* Finish the stream (flush and finalize). */
    obi_status (*finish)(void* ctx);

    void (*destroy)(void* ctx);
} obi_serde_emitter_api_v0;

struct obi_serde_emitter_v0 {
    const obi_serde_emitter_api_v0* api;
    void* ctx;
};

typedef struct obi_data_serde_emit_v0 obi_data_serde_emit_v0;

typedef struct obi_data_serde_emit_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create an emitter that writes to a caller-owned writer.
     *
     * The provider MUST NOT destroy the writer.
     */
    obi_status (*open_writer)(void* ctx,
                              obi_writer_v0 writer,
                              const obi_serde_emit_open_params_v0* params,
                              obi_serde_emitter_v0* out_emitter);
} obi_data_serde_emit_api_v0;

struct obi_data_serde_emit_v0 {
    const obi_data_serde_emit_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_SERDE_EMIT_V0_H */

