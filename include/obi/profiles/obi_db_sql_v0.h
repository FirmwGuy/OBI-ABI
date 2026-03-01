/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DB_SQL_V0_H
#define OBI_PROFILE_DB_SQL_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DB_SQL_V0 "obi.profile:db.sql-0"

enum {
    OBI_DB_SQL_CAP_NAMED_PARAMS  = 1ull << 0,
    OBI_DB_SQL_CAP_OPTIONS_JSON = 1ull << 1,
};

enum {
    OBI_SQL_OPEN_READ_ONLY = 1u << 0,
    OBI_SQL_OPEN_CREATE    = 1u << 1,
};

typedef struct obi_sql_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Provider-defined path/URI (UTF-8, NUL-terminated). Commonly a filesystem path. */
    const char* uri;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_sql_open_params_v0;

typedef enum obi_sql_value_kind_v0 {
    OBI_SQL_VALUE_NULL    = 0,
    OBI_SQL_VALUE_INT64   = 1,
    OBI_SQL_VALUE_DOUBLE  = 2,
    OBI_SQL_VALUE_TEXT    = 3,
    OBI_SQL_VALUE_BLOB    = 4,
} obi_sql_value_kind_v0;

typedef struct obi_sql_stmt_v0 obi_sql_stmt_v0;

typedef struct obi_sql_stmt_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Binding is 1-based (SQLite style). */
    obi_status (*bind_null)(void* ctx, uint32_t index);
    obi_status (*bind_int64)(void* ctx, uint32_t index, int64_t v);
    obi_status (*bind_double)(void* ctx, uint32_t index, double v);

    /* Text is UTF-8 bytes (borrowed for duration of the call). */
    obi_status (*bind_text_utf8)(void* ctx, uint32_t index, obi_utf8_view_v0 text);

    /* Blob bytes are borrowed for duration of the call. */
    obi_status (*bind_blob)(void* ctx, uint32_t index, obi_bytes_view_v0 blob);

    /* Optional: map a parameter name (UTF-8 NUL-terminated) to an index. */
    obi_status (*bind_parameter_index)(void* ctx, const char* name, uint32_t* out_index);

    /* Advance the statement. Returns OK and sets out_has_row.
     * When out_has_row is false, the statement is complete (DONE).
     */
    obi_status (*step)(void* ctx, bool* out_has_row);

    /* Reset the statement to run again (keeps bindings unless cleared). */
    obi_status (*reset)(void* ctx);

    /* Clear all current bindings. */
    obi_status (*clear_bindings)(void* ctx);

    /* Column access is 0-based. Column values are valid until the next step/reset/destroy call. */
    obi_status (*column_count)(void* ctx, uint32_t* out_count);
    obi_status (*column_type)(void* ctx, uint32_t col, obi_sql_value_kind_v0* out_kind);

    obi_status (*column_int64)(void* ctx, uint32_t col, int64_t* out_v);
    obi_status (*column_double)(void* ctx, uint32_t col, double* out_v);
    obi_status (*column_text_utf8)(void* ctx, uint32_t col, obi_utf8_view_v0* out_text);
    obi_status (*column_blob)(void* ctx, uint32_t col, obi_bytes_view_v0* out_blob);

    void (*destroy)(void* ctx);
} obi_sql_stmt_api_v0;

struct obi_sql_stmt_v0 {
    const obi_sql_stmt_api_v0* api;
    void* ctx;
};

typedef struct obi_sql_conn_v0 obi_sql_conn_v0;

typedef struct obi_sql_conn_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Prepare a statement. @sql is UTF-8 NUL-terminated. */
    obi_status (*prepare)(void* ctx, const char* sql, obi_sql_stmt_v0* out_stmt);

    /* Execute a SQL string that does not return rows (DDL/DML). */
    obi_status (*exec)(void* ctx, const char* sql);

    /* Optional: provider-owned error string view (valid until next call). */
    obi_status (*last_error_utf8)(void* ctx, obi_utf8_view_v0* out_err);

    void (*destroy)(void* ctx);
} obi_sql_conn_api_v0;

struct obi_sql_conn_v0 {
    const obi_sql_conn_api_v0* api;
    void* ctx;
};

typedef struct obi_db_sql_v0 obi_db_sql_v0;

typedef struct obi_db_sql_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open)(void* ctx,
                       const obi_sql_open_params_v0* params,
                       obi_sql_conn_v0* out_conn);
} obi_db_sql_api_v0;

struct obi_db_sql_v0 {
    const obi_db_sql_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DB_SQL_V0_H */

