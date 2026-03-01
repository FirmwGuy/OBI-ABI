/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_OS_PROCESS_V0_H
#define OBI_PROFILE_OS_PROCESS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_OS_PROCESS_V0 "obi.profile:os.process-0"

enum {
    OBI_PROCESS_CAP_STDIO_PIPES    = 1ull << 0,
    OBI_PROCESS_CAP_WORKING_DIR    = 1ull << 1,
    OBI_PROCESS_CAP_ENV_OVERRIDES  = 1ull << 2,
    OBI_PROCESS_CAP_KILL           = 1ull << 3,
    OBI_PROCESS_CAP_CANCEL         = 1ull << 4,
    OBI_PROCESS_CAP_OPTIONS_JSON   = 1ull << 5,
};

enum {
    OBI_PROCESS_SPAWN_STDIN_PIPE       = 1u << 0,
    OBI_PROCESS_SPAWN_STDOUT_PIPE      = 1u << 1,
    OBI_PROCESS_SPAWN_STDERR_PIPE      = 1u << 2,
    OBI_PROCESS_SPAWN_STDERR_TO_STDOUT = 1u << 3,

    /* If set, start from an empty environment before applying env_overrides. */
    OBI_PROCESS_SPAWN_CLEAR_ENV        = 1u << 4,
};

typedef struct obi_process_env_kv_v0 {
    /* UTF-8 key (borrowed). */
    obi_utf8_view_v0 key;

    /* UTF-8 value (borrowed).
     * Convention: if value.data==NULL and value.size==0, the variable is removed.
     */
    obi_utf8_view_v0 value;
} obi_process_env_kv_v0;

typedef struct obi_process_spawn_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Program path or name (borrowed). */
    obi_utf8_view_v0 program;

    /* Argument vector (borrowed). Providers SHOULD treat argv[0] as the program display name when
     * present, but MUST use @program to locate the executable.
     */
    const obi_utf8_view_v0* argv;
    size_t argc;

    /* Optional working directory (borrowed). Empty means provider default/inherit. */
    obi_utf8_view_v0 working_dir;

    /* Optional environment overrides (borrowed). */
    const obi_process_env_kv_v0* env_overrides;
    size_t env_overrides_count;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_process_spawn_params_v0;

typedef struct obi_process_v0 obi_process_v0;

typedef struct obi_process_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Wait for the process to exit.
     *
     * Returns OK and sets out_exited:
     * - If timeout_ns==0, this call MUST be non-blocking (poll semantics).
     * - If the process exits, out_exited is true and out_exit_code is set.
     * - If the process has not exited, out_exited is false and out_exit_code is undefined.
     *
     * Cancellation:
     * - If cancel_token.api != NULL and OBI_PROCESS_CAP_CANCEL is supported, providers SHOULD
     *   return CANCELLED when cancellation is requested.
     */
    obi_status (*wait)(void* ctx,
                       uint64_t timeout_ns,
                       obi_cancel_token_v0 cancel_token,
                       bool* out_exited,
                       int32_t* out_exit_code);

    /* Optional: request process termination. */
    obi_status (*kill)(void* ctx);

    void (*destroy)(void* ctx);
} obi_process_api_v0;

struct obi_process_v0 {
    const obi_process_api_v0* api;
    void* ctx;
};

typedef struct obi_os_process_v0 obi_os_process_v0;

typedef struct obi_os_process_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Spawn a process.
     *
     * Stdio:
     * - If a *_PIPE spawn flag is set and supported, the corresponding stream out parameter is
     *   populated with a provider-owned reader/writer.
     * - Otherwise, the provider MUST set out_*->api = NULL and out_*->ctx = NULL.
     */
    obi_status (*spawn)(void* ctx,
                        const obi_process_spawn_params_v0* params,
                        obi_process_v0* out_process,
                        obi_writer_v0* out_stdin,
                        obi_reader_v0* out_stdout,
                        obi_reader_v0* out_stderr);
} obi_os_process_api_v0;

struct obi_os_process_v0 {
    const obi_os_process_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_OS_PROCESS_V0_H */

