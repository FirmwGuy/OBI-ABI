/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CORE_WAITSET_V0_H
#define OBI_PROFILE_CORE_WAITSET_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CORE_WAITSET_V0 "obi.profile:core.waitset-0"

enum {
    OBI_WAITSET_CAP_FD         = 1ull << 0,
    OBI_WAITSET_CAP_WIN_HANDLE = 1ull << 1,
};

enum {
    OBI_WAIT_EVENT_READ  = 1u << 0,
    OBI_WAIT_EVENT_WRITE = 1u << 1,
    OBI_WAIT_EVENT_ERROR = 1u << 2,
};

typedef enum obi_wait_handle_kind_v0 {
    OBI_WAIT_HANDLE_FD         = 0,
    OBI_WAIT_HANDLE_WIN_HANDLE = 1,
} obi_wait_handle_kind_v0;

typedef struct obi_wait_handle_v0 {
    uint32_t kind;   /* obi_wait_handle_kind_v0 */
    uint32_t events; /* OBI_WAIT_EVENT_* mask */

    /* For FD: this is the integer fd value.
     * For WIN_HANDLE: this is the HANDLE value cast to int64_t.
     */
    int64_t handle;

    /* Provider-defined stable tag for correlation/debug (0 if unused). */
    uint64_t tag;
} obi_wait_handle_v0;

typedef struct obi_waitset_v0 obi_waitset_v0;

typedef struct obi_waitset_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Query the provider's current wait handles and a suggested timeout.
     *
     * - out_timeout_ns is a suggested *relative* timeout in nanoseconds (monotonic).
     * - out_timeout_ns may be 0 to indicate "poll" or "no wait".
     * - out_timeout_ns may be UINT64_MAX to indicate "no timeout known / wait indefinitely".
     */
    obi_status (*get_wait_handles)(void* ctx,
                                   obi_wait_handle_v0* handles,
                                   size_t handle_cap,
                                   size_t* out_handle_count,
                                   uint64_t* out_timeout_ns);
} obi_waitset_api_v0;

struct obi_waitset_v0 {
    const obi_waitset_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CORE_WAITSET_V0_H */

