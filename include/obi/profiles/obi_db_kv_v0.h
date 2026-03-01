/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DB_KV_V0_H
#define OBI_PROFILE_DB_KV_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DB_KV_V0 "obi.profile:db.kv-0"

enum {
    OBI_DB_KV_CAP_CURSOR       = 1ull << 0,
    OBI_DB_KV_CAP_OPTIONS_JSON = 1ull << 1,
};

enum {
    OBI_KV_DB_OPEN_READ_ONLY = 1u << 0,
    OBI_KV_DB_OPEN_CREATE    = 1u << 1,
};

typedef struct obi_kv_db_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Provider-defined path/URI (UTF-8, NUL-terminated). Commonly a filesystem path. */
    const char* path;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_kv_db_open_params_v0;

enum {
    OBI_KV_TXN_READ_ONLY = 1u << 0,
};

typedef struct obi_kv_txn_params_v0 {
    uint32_t struct_size;
    uint32_t flags;
} obi_kv_txn_params_v0;

typedef struct obi_kv_cursor_v0 obi_kv_cursor_v0;

typedef struct obi_kv_cursor_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Position cursor at the first entry. Returns OK and sets out_has_item. */
    obi_status (*first)(void* ctx, bool* out_has_item);

    /* Position cursor at the first entry with key >= @key. Returns OK and sets out_has_item. */
    obi_status (*seek_ge)(void* ctx, obi_bytes_view_v0 key, bool* out_has_item);

    /* Advance to next entry. Returns OK and sets out_has_item. */
    obi_status (*next)(void* ctx, bool* out_has_item);

    /* Copy current key/value into host buffers.
     *
     * If out_* is NULL or too small, return BUFFER_TOO_SMALL and set out_size to required bytes.
     */
    obi_status (*key)(void* ctx, void* out_key, size_t out_cap, size_t* out_size);
    obi_status (*value)(void* ctx, void* out_value, size_t out_cap, size_t* out_size);

    void (*destroy)(void* ctx);
} obi_kv_cursor_api_v0;

struct obi_kv_cursor_v0 {
    const obi_kv_cursor_api_v0* api;
    void* ctx;
};

typedef struct obi_kv_txn_v0 obi_kv_txn_v0;

typedef struct obi_kv_txn_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Lookup a value by key. Returns OK and sets out_found.
     *
     * If found and out_value is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     * When not found, out_size MUST be set to 0.
     */
    obi_status (*get)(void* ctx,
                      obi_bytes_view_v0 key,
                      void* out_value,
                      size_t out_cap,
                      size_t* out_size,
                      bool* out_found);

    /* Insert or replace a value. */
    obi_status (*put)(void* ctx, obi_bytes_view_v0 key, obi_bytes_view_v0 value);

    /* Delete a key if present. Returns OK and sets out_deleted. */
    obi_status (*del)(void* ctx, obi_bytes_view_v0 key, bool* out_deleted);

    /* Optional cursor creation for iteration. */
    obi_status (*cursor_open)(void* ctx, obi_kv_cursor_v0* out_cursor);

    /* Commit the transaction. After commit, the txn handle is no longer usable. */
    obi_status (*commit)(void* ctx);

    /* Abort/rollback the transaction. After abort, the txn handle is no longer usable. */
    void (*abort)(void* ctx);

    void (*destroy)(void* ctx);
} obi_kv_txn_api_v0;

struct obi_kv_txn_v0 {
    const obi_kv_txn_api_v0* api;
    void* ctx;
};

typedef struct obi_kv_db_v0 obi_kv_db_v0;

typedef struct obi_kv_db_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*begin_txn)(void* ctx,
                            const obi_kv_txn_params_v0* params,
                            obi_kv_txn_v0* out_txn);

    void (*destroy)(void* ctx);
} obi_kv_db_api_v0;

struct obi_kv_db_v0 {
    const obi_kv_db_api_v0* api;
    void* ctx;
};

typedef struct obi_db_kv_v0 obi_db_kv_v0;

typedef struct obi_db_kv_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open)(void* ctx,
                       const obi_kv_db_open_params_v0* params,
                       obi_kv_db_v0* out_db);
} obi_db_kv_api_v0;

struct obi_db_kv_v0 {
    const obi_db_kv_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DB_KV_V0_H */

