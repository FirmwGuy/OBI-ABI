/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CORE_CANCEL_V0_H
#define OBI_PROFILE_CORE_CANCEL_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CORE_CANCEL_V0 "obi.profile:core.cancel-0"

enum {
    OBI_CANCEL_PROFILE_CAP_RESET = 1ull << 0,
};

typedef struct obi_cancel_source_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_cancel_source_params_v0;

typedef struct obi_cancel_source_v0 obi_cancel_source_v0;

typedef struct obi_cancel_source_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Obtain a cancellation token associated with this source.
     *
     * Lifetime:
     * - The returned token is provider-owned and must be destroyed by the caller.
     * - Cancelling the source MUST cause token.is_cancelled() to return true.
     */
    obi_status (*token)(void* ctx, obi_cancel_token_v0* out_token);

    /* Request cancellation with an optional reason (UTF-8). */
    obi_status (*cancel)(void* ctx, obi_utf8_view_v0 reason);

    /* Optional: reset source back to "not cancelled". */
    obi_status (*reset)(void* ctx);

    void (*destroy)(void* ctx);
} obi_cancel_source_api_v0;

struct obi_cancel_source_v0 {
    const obi_cancel_source_api_v0* api;
    void* ctx;
};

typedef struct obi_cancel_v0 obi_cancel_v0;

typedef struct obi_cancel_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*create_source)(void* ctx,
                                const obi_cancel_source_params_v0* params,
                                obi_cancel_source_v0* out_source);
} obi_cancel_api_v0;

struct obi_cancel_v0 {
    const obi_cancel_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CORE_CANCEL_V0_H */

