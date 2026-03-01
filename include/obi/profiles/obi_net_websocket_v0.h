/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_WEBSOCKET_V0_H
#define OBI_PROFILE_NET_WEBSOCKET_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_NET_WEBSOCKET_V0 "obi.profile:net.websocket-0"

enum {
    /* Provider supports connect_async and returns jobs that can be polled. */
    OBI_WS_CAP_ASYNC_CONNECT  = 1ull << 0,
    /* Async operations progress only via a pump profile (no internal threads). */
    OBI_WS_CAP_REQUIRES_PUMP  = 1ull << 1,
};

typedef enum obi_ws_opcode_v0 {
    OBI_WS_TEXT   = 1,
    OBI_WS_BINARY = 2,
    OBI_WS_CLOSE  = 8,
    OBI_WS_PING   = 9,
    OBI_WS_PONG   = 10,
} obi_ws_opcode_v0;

typedef enum obi_ws_payload_kind_v0 {
    OBI_WS_PAYLOAD_NONE   = 0,
    OBI_WS_PAYLOAD_BYTES  = 1,
    OBI_WS_PAYLOAD_READER = 2,
} obi_ws_payload_kind_v0;

typedef struct obi_ws_payload_v0 {
    obi_ws_payload_kind_v0 kind;
    uint32_t reserved;
    union {
        obi_bytes_view_v0 as_bytes;
        obi_reader_v0 as_reader;
    } u;
} obi_ws_payload_v0;

typedef struct obi_ws_header_kv_v0 {
    const char* key;
    const char* value;
} obi_ws_header_kv_v0;

typedef struct obi_ws_headers_view_v0 {
    const obi_ws_header_kv_v0* items;
    size_t count;
} obi_ws_headers_view_v0;

typedef struct obi_ws_connect_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Required. */
    const char* url;

    /* Optional extra headers (UTF-8 key/value pairs). */
    obi_ws_headers_view_v0 headers;

    /* Optional subprotocol (UTF-8, NUL-terminated). */
    const char* subprotocol;

    /* Timeout for connect; 0 means provider default. */
    uint64_t timeout_ns;
} obi_ws_connect_params_v0;

typedef struct obi_ws_message_v0 {
    obi_ws_opcode_v0 opcode;
    uint16_t close_code; /* only meaningful for CLOSE */
    uint16_t reserved16;

    /* Provider-owned payload reader; may be {NULL,NULL} for empty payload. */
    obi_reader_v0 payload;

    /* Provider-owned message state, released by release(). */
    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_ws_message_v0* msg);
} obi_ws_message_v0;

typedef struct obi_ws_conn_v0 obi_ws_conn_v0;

typedef struct obi_ws_conn_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Send a message. */
    obi_status (*send)(void* ctx,
                       obi_ws_opcode_v0 opcode,
                       const obi_ws_payload_v0* payload,
                       uint64_t timeout_ns,
                       uint64_t* out_bytes_sent);

    /* Receive the next message. Returns OK and sets out_has_msg. */
    obi_status (*receive)(void* ctx,
                          uint64_t timeout_ns,
                          obi_ws_message_v0* out_msg,
                          bool* out_has_msg);

    /* Request a close with a code and optional UTF-8 reason string. */
    obi_status (*close)(void* ctx, uint16_t code, const char* reason_utf8);

    void (*destroy)(void* ctx);
} obi_ws_conn_api_v0;

struct obi_ws_conn_v0 {
    const obi_ws_conn_api_v0* api;
    void* ctx;
};

typedef struct obi_ws_connect_job_v0 obi_ws_connect_job_v0;

typedef struct obi_ws_connect_job_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Poll for completion. Returns OK and sets out_done. */
    obi_status (*poll)(void* ctx, uint64_t timeout_ns, bool* out_done);

    /* When complete, transfer ownership of the connection into out_conn. */
    obi_status (*take_conn)(void* ctx, obi_ws_conn_v0* out_conn);

    void (*cancel)(void* ctx);
    void (*destroy)(void* ctx);
} obi_ws_connect_job_api_v0;

struct obi_ws_connect_job_v0 {
    const obi_ws_connect_job_api_v0* api;
    void* ctx;
};

typedef struct obi_net_websocket_v0 obi_net_websocket_v0;

typedef struct obi_net_websocket_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*connect)(void* ctx,
                          const obi_ws_connect_params_v0* params,
                          obi_ws_conn_v0* out_conn);

    obi_status (*connect_async)(void* ctx,
                                const obi_ws_connect_params_v0* params,
                                obi_ws_connect_job_v0* out_job);
} obi_net_websocket_api_v0;

struct obi_net_websocket_v0 {
    const obi_net_websocket_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_WEBSOCKET_V0_H */
