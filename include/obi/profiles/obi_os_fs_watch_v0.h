/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_OS_FS_WATCH_V0_H
#define OBI_PROFILE_OS_FS_WATCH_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_OS_FS_WATCH_V0 "obi.profile:os.fs_watch-0"

enum {
    OBI_FS_WATCH_CAP_RECURSIVE   = 1ull << 0,
    OBI_FS_WATCH_CAP_RENAME_PAIR = 1ull << 1,
    OBI_FS_WATCH_CAP_OPTIONS_JSON = 1ull << 2,
};

typedef enum obi_fs_watch_event_kind_v0 {
    OBI_FS_WATCH_EVENT_CREATE = 0,
    OBI_FS_WATCH_EVENT_MODIFY = 1,
    OBI_FS_WATCH_EVENT_REMOVE = 2,
    OBI_FS_WATCH_EVENT_RENAME = 3,
    OBI_FS_WATCH_EVENT_ATTRIB = 4,
    OBI_FS_WATCH_EVENT_OTHER  = 5,
} obi_fs_watch_event_kind_v0;

enum {
    OBI_FS_WATCH_EVENT_FLAG_IS_DIR   = 1u << 0,
    /* Provider detected overflow/dropped events. */
    OBI_FS_WATCH_EVENT_FLAG_OVERFLOW = 1u << 1,
};

typedef struct obi_fs_watch_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_fs_watch_open_params_v0;

enum {
    OBI_FS_WATCH_ADD_RECURSIVE = 1u << 0,
};

typedef struct obi_fs_watch_add_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Required path (UTF-8, NUL-terminated). */
    const char* path;
} obi_fs_watch_add_params_v0;

typedef struct obi_fs_watch_event_v0 {
    obi_fs_watch_event_kind_v0 kind;
    uint32_t flags;

    uint64_t watch_id;
    uint64_t cookie;

    /* Provider-owned UTF-8 views. Valid until batch release. */
    obi_utf8_view_v0 path;
    obi_utf8_view_v0 path2; /* used for rename pairs when available; empty otherwise */
} obi_fs_watch_event_v0;

typedef struct obi_fs_watch_event_batch_v0 {
    const obi_fs_watch_event_v0* events;
    size_t count;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_fs_watch_event_batch_v0* batch);
} obi_fs_watch_event_batch_v0;

typedef struct obi_fs_watcher_v0 obi_fs_watcher_v0;

typedef struct obi_fs_watcher_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Add a watch and return a provider-defined watch ID. */
    obi_status (*add_watch)(void* ctx,
                            const obi_fs_watch_add_params_v0* params,
                            uint64_t* out_watch_id);

    /* Remove a watch by ID. */
    obi_status (*remove_watch)(void* ctx, uint64_t watch_id);

    /* Poll for a batch of events. Returns OK and sets out_has_batch. */
    obi_status (*poll_events)(void* ctx,
                              uint64_t timeout_ns,
                              obi_fs_watch_event_batch_v0* out_batch,
                              bool* out_has_batch);

    void (*destroy)(void* ctx);
} obi_fs_watcher_api_v0;

struct obi_fs_watcher_v0 {
    const obi_fs_watcher_api_v0* api;
    void* ctx;
};

typedef struct obi_os_fs_watch_v0 obi_os_fs_watch_v0;

typedef struct obi_os_fs_watch_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_watcher)(void* ctx,
                               const obi_fs_watch_open_params_v0* params,
                               obi_fs_watcher_v0* out_watcher);
} obi_os_fs_watch_api_v0;

struct obi_os_fs_watch_v0 {
    const obi_os_fs_watch_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_OS_FS_WATCH_V0_H */

