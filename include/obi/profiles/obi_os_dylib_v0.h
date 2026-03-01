/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_OS_DYLIB_V0_H
#define OBI_PROFILE_OS_DYLIB_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_OS_DYLIB_V0 "obi.profile:os.dylib-0"

enum {
    OBI_DYLIB_CAP_OPEN_SELF    = 1ull << 0,
    OBI_DYLIB_CAP_OPTIONS_JSON = 1ull << 1,
};

enum {
    OBI_DYLIB_OPEN_NOW    = 1u << 0,
    OBI_DYLIB_OPEN_GLOBAL = 1u << 1,
};

typedef struct obi_dylib_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_dylib_open_params_v0;

typedef struct obi_dylib_v0 obi_dylib_v0;

typedef struct obi_dylib_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Lookup a symbol by name (UTF-8, NUL-terminated). Returns OK and sets out_found. */
    obi_status (*sym)(void* ctx, const char* name, void** out_sym, bool* out_found);

    void (*destroy)(void* ctx);
} obi_dylib_api_v0;

struct obi_dylib_v0 {
    const obi_dylib_api_v0* api;
    void* ctx;
};

typedef struct obi_os_dylib_v0 obi_os_dylib_v0;

typedef struct obi_os_dylib_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Open a dynamic library by path (UTF-8, NUL-terminated). If path==NULL and OPEN_SELF is
     * supported, providers open the current process module.
     */
    obi_status (*open)(void* ctx,
                       const char* path,
                       const obi_dylib_open_params_v0* params,
                       obi_dylib_v0* out_lib);
} obi_os_dylib_api_v0;

struct obi_os_dylib_v0 {
    const obi_os_dylib_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_OS_DYLIB_V0_H */

