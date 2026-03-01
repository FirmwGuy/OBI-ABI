/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_ARCHIVE_V0_H
#define OBI_PROFILE_DATA_ARCHIVE_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_ARCHIVE_V0 "obi.profile:data.archive-0"

enum {
    /* Provider supports open_writer and archive writing. */
    OBI_ARCHIVE_CAP_WRITE        = 1ull << 0,
    /* Provider supports skip_entry on archive readers. */
    OBI_ARCHIVE_CAP_SKIP_ENTRY   = 1ull << 1,
    /* Provider supports provider-specific options_json. */
    OBI_ARCHIVE_CAP_OPTIONS_JSON = 1ull << 2,
};

typedef enum obi_archive_entry_kind_v0 {
    OBI_ARCHIVE_ENTRY_FILE    = 0,
    OBI_ARCHIVE_ENTRY_DIR     = 1,
    OBI_ARCHIVE_ENTRY_SYMLINK = 2,
    OBI_ARCHIVE_ENTRY_OTHER   = 3,
} obi_archive_entry_kind_v0;

typedef struct obi_archive_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint, UTF-8 NUL-terminated (examples: "zip", "tar", "7z"). */
    const char* format_hint;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_archive_open_params_v0;

typedef struct obi_archive_entry_v0 {
    obi_archive_entry_kind_v0 kind;
    uint32_t reserved;

    /* Provider-owned path view. Valid until the next next_entry call or reader destruction. */
    obi_utf8_view_v0 path;

    /* Uncompressed size if known; otherwise 0 and flags may indicate unknown. */
    uint64_t size_bytes;

    /* Unix epoch nanoseconds when known; 0 if unknown. */
    uint64_t mtime_unix_ns;

    /* POSIX mode bits when known; 0 if unknown. */
    uint32_t posix_mode;
    uint32_t reserved2;

    /* Provider-owned symlink target view (only for SYMLINK). Empty if not applicable. */
    obi_utf8_view_v0 symlink_target;
} obi_archive_entry_v0;

typedef struct obi_archive_reader_v0 obi_archive_reader_v0;

typedef struct obi_archive_reader_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Enumerate the next entry. Returns OK and sets out_has_entry. */
    obi_status (*next_entry)(void* ctx, obi_archive_entry_v0* out_entry, bool* out_has_entry);

    /* Open a reader for the current entry's payload. */
    obi_status (*open_entry_reader)(void* ctx, obi_reader_v0* out_entry_reader);

    /* Optional: skip/drain the current entry without reading it. */
    obi_status (*skip_entry)(void* ctx);

    void (*destroy)(void* ctx);
} obi_archive_reader_api_v0;

struct obi_archive_reader_v0 {
    const obi_archive_reader_api_v0* api;
    void* ctx;
};

typedef struct obi_archive_entry_create_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_archive_entry_kind_v0 kind;
    uint32_t reserved;

    /* Required UTF-8 NUL-terminated entry path. */
    const char* path;

    /* Optional for files; may be 0 if unknown. */
    uint64_t size_bytes;

    /* Optional; 0 means unknown. */
    uint64_t mtime_unix_ns;

    /* Optional; 0 means provider default. */
    uint32_t posix_mode;
    uint32_t reserved2;

    /* Optional for symlinks; UTF-8 NUL-terminated. */
    const char* symlink_target;
} obi_archive_entry_create_v0;

typedef struct obi_archive_writer_v0 obi_archive_writer_v0;

typedef struct obi_archive_writer_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Begin a new entry. Returns a provider-owned writer for the entry payload. */
    obi_status (*begin_entry)(void* ctx,
                              const obi_archive_entry_create_v0* entry,
                              obi_writer_v0* out_entry_writer);

    /* Finish writing the archive (optional; destroy should also finalize). */
    obi_status (*finish)(void* ctx);

    void (*destroy)(void* ctx);
} obi_archive_writer_api_v0;

struct obi_archive_writer_v0 {
    const obi_archive_writer_api_v0* api;
    void* ctx;
};

typedef struct obi_data_archive_v0 obi_data_archive_v0;

typedef struct obi_data_archive_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 src,
                              const obi_archive_open_params_v0* params,
                              obi_archive_reader_v0* out_reader);

    obi_status (*open_writer)(void* ctx,
                              obi_writer_v0 dst,
                              const obi_archive_open_params_v0* params,
                              obi_archive_writer_v0* out_writer);
} obi_data_archive_api_v0;

struct obi_data_archive_v0 {
    const obi_data_archive_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_ARCHIVE_V0_H */

