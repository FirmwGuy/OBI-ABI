/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_OS_FS_V0_H
#define OBI_PROFILE_OS_FS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_OS_FS_V0 "obi.profile:os.fs-0"

enum {
    OBI_FS_CAP_DIR_ITER     = 1ull << 0,
    OBI_FS_CAP_OPTIONS_JSON = 1ull << 1,
};

typedef struct obi_fs_open_reader_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_fs_open_reader_params_v0;

enum {
    OBI_FS_OPEN_WRITE_CREATE     = 1u << 0,
    OBI_FS_OPEN_WRITE_TRUNCATE   = 1u << 1,
    OBI_FS_OPEN_WRITE_APPEND     = 1u << 2,
    OBI_FS_OPEN_WRITE_EXCLUSIVE  = 1u << 3,
};

typedef struct obi_fs_open_writer_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_fs_open_writer_params_v0;

typedef enum obi_fs_entry_kind_v0 {
    OBI_FS_ENTRY_FILE    = 0,
    OBI_FS_ENTRY_DIR     = 1,
    OBI_FS_ENTRY_SYMLINK = 2,
    OBI_FS_ENTRY_OTHER   = 3,
} obi_fs_entry_kind_v0;

typedef struct obi_fs_stat_v0 {
    obi_fs_entry_kind_v0 kind;
    uint32_t reserved;

    uint64_t size_bytes;

    /* Unix epoch nanoseconds when known; 0 if unknown. */
    uint64_t mtime_unix_ns;

    /* POSIX mode bits when known; 0 if unknown. */
    uint32_t posix_mode;
    uint32_t reserved2;
} obi_fs_stat_v0;

typedef struct obi_fs_dir_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_fs_dir_open_params_v0;

typedef struct obi_fs_dir_entry_v0 {
    obi_fs_entry_kind_v0 kind;
    uint32_t reserved;

    /* Provider-owned views. Valid until the next_entry call or iterator destruction. */
    obi_utf8_view_v0 name;
    obi_utf8_view_v0 full_path;

    /* Size when known (files); 0 if unknown/not applicable. */
    uint64_t size_bytes;
} obi_fs_dir_entry_v0;

enum {
    OBI_FS_MKDIR_RECURSIVE = 1u << 0,
};

enum {
    /* If set, replace an existing destination when supported. If clear, providers SHOULD
     * fail when the destination exists.
     */
    OBI_FS_RENAME_REPLACE = 1u << 0,
};

typedef struct obi_fs_dir_iter_v0 obi_fs_dir_iter_v0;

typedef struct obi_fs_dir_iter_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Enumerate the next entry. Returns OK and sets out_has_entry. */
    obi_status (*next_entry)(void* ctx, obi_fs_dir_entry_v0* out_entry, bool* out_has_entry);

    void (*destroy)(void* ctx);
} obi_fs_dir_iter_api_v0;

struct obi_fs_dir_iter_v0 {
    const obi_fs_dir_iter_api_v0* api;
    void* ctx;
};

typedef struct obi_os_fs_v0 obi_os_fs_v0;

typedef struct obi_os_fs_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Open a provider-owned reader for a path/URI (UTF-8, NUL-terminated). */
    obi_status (*open_reader)(void* ctx,
                              const char* path,
                              const obi_fs_open_reader_params_v0* params,
                              obi_reader_v0* out_reader);

    /* Open a provider-owned writer for a path/URI (UTF-8, NUL-terminated). */
    obi_status (*open_writer)(void* ctx,
                              const char* path,
                              const obi_fs_open_writer_params_v0* params,
                              obi_writer_v0* out_writer);

    /* Stat a path. Returns OK and sets out_found. */
    obi_status (*stat)(void* ctx, const char* path, obi_fs_stat_v0* out_stat, bool* out_found);

    /* Create a directory (optionally recursive via flags). */
    obi_status (*mkdir)(void* ctx, const char* path, uint32_t flags);

    /* Remove a file or empty directory. Returns OK and sets out_removed. */
    obi_status (*remove)(void* ctx, const char* path, bool* out_removed);

    /* Rename/move a path. */
    obi_status (*rename)(void* ctx, const char* from_path, const char* to_path, uint32_t flags);

    /* Optional directory iterator. */
    obi_status (*open_dir_iter)(void* ctx,
                                const char* path,
                                const obi_fs_dir_open_params_v0* params,
                                obi_fs_dir_iter_v0* out_iter);
} obi_os_fs_api_v0;

struct obi_os_fs_v0 {
    const obi_os_fs_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_OS_FS_V0_H */
