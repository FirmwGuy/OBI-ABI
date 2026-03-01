/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_OS_ENV_V0_H
#define OBI_PROFILE_OS_ENV_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_OS_ENV_V0 "obi.profile:os.env-0"

enum {
    OBI_ENV_CAP_SET        = 1ull << 0,
    OBI_ENV_CAP_CWD        = 1ull << 1,
    OBI_ENV_CAP_CHDIR      = 1ull << 2,
    OBI_ENV_CAP_KNOWN_DIRS = 1ull << 3,
    OBI_ENV_CAP_ENUM       = 1ull << 4,
};

enum {
    /* Set/unset: fail if the variable already exists (when supported). */
    OBI_ENV_SET_NO_OVERWRITE = 1u << 0,
};

typedef enum obi_env_known_dir_kind_v0 {
    OBI_ENV_KNOWN_DIR_HOME          = 0,
    OBI_ENV_KNOWN_DIR_TEMP          = 1,
    OBI_ENV_KNOWN_DIR_USER_CONFIG   = 2,
    OBI_ENV_KNOWN_DIR_USER_DATA     = 3,
    OBI_ENV_KNOWN_DIR_USER_CACHE    = 4,
    OBI_ENV_KNOWN_DIR_SYSTEM_CONFIG = 5,
    OBI_ENV_KNOWN_DIR_SYSTEM_DATA   = 6,
} obi_env_known_dir_kind_v0;

typedef struct obi_env_iter_v0 obi_env_iter_v0;

typedef struct obi_env_iter_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Returns OK and sets out_has_item. out_name/out_value are provider-owned views valid until
     * the next next() call or iterator destruction.
     */
    obi_status (*next)(void* ctx,
                       obi_utf8_view_v0* out_name,
                       obi_utf8_view_v0* out_value,
                       bool* out_has_item);

    void (*destroy)(void* ctx);
} obi_env_iter_api_v0;

struct obi_env_iter_v0 {
    const obi_env_iter_api_v0* api;
    void* ctx;
};

typedef struct obi_os_env_v0 obi_os_env_v0;

typedef struct obi_os_env_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Get an environment variable value as UTF-8 bytes.
     *
     * Returns OK and sets out_found:
     * - If not found, out_size MUST be set to 0.
     * - If found and out_value is NULL or too small, return BUFFER_TOO_SMALL and set out_size to
     *   required bytes.
     *
     * Implementations SHOULD NUL-terminate out_value when out_cap > out_size.
     */
    obi_status (*getenv_utf8)(void* ctx,
                              const char* name,
                              char* out_value,
                              size_t out_cap,
                              size_t* out_size,
                              bool* out_found);

    /* Optional: set or unset an environment variable. */
    obi_status (*setenv_utf8)(void* ctx, const char* name, const char* value, uint32_t flags);
    obi_status (*unsetenv)(void* ctx, const char* name);

    /* Optional: get or set the current working directory. */
    obi_status (*get_cwd_utf8)(void* ctx, char* out_path, size_t out_cap, size_t* out_size);
    obi_status (*chdir)(void* ctx, const char* path);

    /* Optional: map a portable directory kind to a provider-defined path. */
    obi_status (*known_dir_utf8)(void* ctx,
                                 obi_env_known_dir_kind_v0 kind,
                                 char* out_path,
                                 size_t out_cap,
                                 size_t* out_size,
                                 bool* out_found);

    /* Optional: iterate environment variables. */
    obi_status (*env_iter_open)(void* ctx, obi_env_iter_v0* out_iter);
} obi_os_env_api_v0;

struct obi_os_env_v0 {
    const obi_os_env_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_OS_ENV_V0_H */

