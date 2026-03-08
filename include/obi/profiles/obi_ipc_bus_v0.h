/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_IPC_BUS_V0_H
#define OBI_PROFILE_IPC_BUS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_IPC_BUS_V0 "obi.profile:ipc.bus-0"

enum {
    OBI_IPC_BUS_CAP_CUSTOM_ADDRESS = 1ull << 0,
    OBI_IPC_BUS_CAP_SIGNAL_EMIT    = 1ull << 1,
    OBI_IPC_BUS_CAP_OWN_NAME       = 1ull << 2,
    OBI_IPC_BUS_CAP_OPTIONS_JSON   = 1ull << 3,
    OBI_IPC_BUS_CAP_CANCEL         = 1ull << 4,
    OBI_IPC_BUS_CAP_REQUIRES_PUMP  = 1ull << 5,
};

typedef enum obi_bus_endpoint_kind_v0 {
    OBI_BUS_ENDPOINT_SESSION = 0,
    OBI_BUS_ENDPOINT_SYSTEM  = 1,
    OBI_BUS_ENDPOINT_CUSTOM  = 2,
} obi_bus_endpoint_kind_v0;

enum {
    OBI_BUS_REQUEST_NAME_REPLACE_EXISTING = 1u << 0,
    OBI_BUS_REQUEST_NAME_DO_NOT_QUEUE     = 1u << 1,
};

typedef struct obi_bus_connect_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint32_t endpoint_kind; /* obi_bus_endpoint_kind_v0 */
    uint32_t reserved;

    /* Used only when endpoint_kind == OBI_BUS_ENDPOINT_CUSTOM. */
    obi_utf8_view_v0 custom_address;

    /* Optional provider-specific JSON object string. */
    obi_utf8_view_v0 options_json;

    uint64_t timeout_ns; /* 0 means provider default. */
} obi_bus_connect_params_v0;

typedef struct obi_bus_call_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_utf8_view_v0 destination_name;
    obi_utf8_view_v0 object_path;
    obi_utf8_view_v0 interface_name;
    obi_utf8_view_v0 member_name;

    /* UTF-8 JSON array of positional arguments. */
    obi_utf8_view_v0 args_json;

    /* Optional provider-specific JSON object string. */
    obi_utf8_view_v0 options_json;

    uint64_t timeout_ns; /* 0 means provider default. */
} obi_bus_call_params_v0;

typedef struct obi_bus_reply_v0 {
    /* Provider-owned UTF-8 JSON array of return values. May be empty when there are no values. */
    obi_utf8_view_v0 results_json;

    /* Optional backend-native remote error name. Empty on success. */
    obi_utf8_view_v0 remote_error_name;

    /* Optional UTF-8 JSON object/array/value with remote error details. */
    obi_utf8_view_v0 error_details_json;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_bus_reply_v0* out_reply);
} obi_bus_reply_v0;

typedef struct obi_bus_signal_filter_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Empty fields are wildcards. */
    obi_utf8_view_v0 sender_name;
    obi_utf8_view_v0 object_path;
    obi_utf8_view_v0 interface_name;
    obi_utf8_view_v0 member_name;
} obi_bus_signal_filter_v0;

typedef struct obi_bus_signal_emit_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_utf8_view_v0 object_path;
    obi_utf8_view_v0 interface_name;
    obi_utf8_view_v0 member_name;

    /* UTF-8 JSON array of positional arguments. */
    obi_utf8_view_v0 args_json;

    /* Optional provider-specific JSON object string. */
    obi_utf8_view_v0 options_json;
} obi_bus_signal_emit_v0;

typedef struct obi_bus_signal_v0 {
    obi_utf8_view_v0 sender_name;
    obi_utf8_view_v0 object_path;
    obi_utf8_view_v0 interface_name;
    obi_utf8_view_v0 member_name;
    obi_utf8_view_v0 args_json; /* provider-owned UTF-8 JSON array */

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_bus_signal_v0* out_signal);
} obi_bus_signal_v0;

typedef struct obi_bus_subscription_v0 obi_bus_subscription_v0;

typedef struct obi_bus_subscription_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Wait for the next matching signal. Returns OK and sets out_has_signal. */
    obi_status (*next)(void* ctx,
                       uint64_t timeout_ns,
                       obi_cancel_token_v0 cancel_token,
                       obi_bus_signal_v0* out_signal,
                       bool* out_has_signal);

    void (*destroy)(void* ctx);
} obi_bus_subscription_api_v0;

struct obi_bus_subscription_v0 {
    const obi_bus_subscription_api_v0* api;
    void* ctx;
};

typedef struct obi_bus_conn_v0 obi_bus_conn_v0;

typedef struct obi_bus_conn_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*call_json)(void* ctx,
                            const obi_bus_call_params_v0* params,
                            obi_cancel_token_v0 cancel_token,
                            obi_bus_reply_v0* out_reply);

    obi_status (*subscribe_signals)(void* ctx,
                                    const obi_bus_signal_filter_v0* filter,
                                    obi_bus_subscription_v0* out_subscription);

    /* Optional: emit a signal on the bus. */
    obi_status (*emit_signal_json)(void* ctx,
                                   const obi_bus_signal_emit_v0* signal);

    /* Optional: request ownership of a well-known bus name. */
    obi_status (*request_name)(void* ctx,
                               obi_utf8_view_v0 name,
                               uint32_t flags,
                               bool* out_acquired);

    /* Optional: release a previously requested well-known bus name. */
    obi_status (*release_name)(void* ctx,
                               obi_utf8_view_v0 name);

    void (*destroy)(void* ctx);
} obi_bus_conn_api_v0;

struct obi_bus_conn_v0 {
    const obi_bus_conn_api_v0* api;
    void* ctx;
};

typedef struct obi_ipc_bus_v0 obi_ipc_bus_v0;

typedef struct obi_ipc_bus_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*connect)(void* ctx,
                          const obi_bus_connect_params_v0* params,
                          obi_cancel_token_v0 cancel_token,
                          obi_bus_conn_v0* out_conn);
} obi_ipc_bus_api_v0;

struct obi_ipc_bus_v0 {
    const obi_ipc_bus_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_IPC_BUS_V0_H */

