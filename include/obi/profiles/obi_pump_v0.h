/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_PUMP_V0_H
#define OBI_PROFILE_PUMP_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CORE_PUMP_V0 "obi.profile:core.pump-0"

typedef struct obi_pump_wait_hint_v0 {
    /* If nonzero, host SHOULD call step within this many nanoseconds to avoid stalling progress. */
    uint64_t next_timeout_ns;
    /* Reserved for future expansion (for example "has work now"). */
    uint64_t flags;
} obi_pump_wait_hint_v0;

typedef struct obi_pump_v0 obi_pump_v0;

typedef struct obi_pump_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Advance internal provider work by up to timeout_ns. Implementations may return earlier. */
    obi_status (*step)(void* ctx, uint64_t timeout_ns);

    /* Optional wait-hint for more efficient integration. */
    obi_status (*get_wait_hint)(void* ctx, obi_pump_wait_hint_v0* out_hint);
} obi_pump_api_v0;

struct obi_pump_v0 {
    const obi_pump_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_PUMP_V0_H */

