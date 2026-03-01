/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_SOCKET_V0_H
#define OBI_PROFILE_NET_SOCKET_V0_H

#include "../obi_core_v0.h"
#include "obi_net_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_NET_SOCKET_V0 "obi.profile:net.socket-0"

enum {
    OBI_SOCKET_CAP_TCP_CONNECT      = 1ull << 0,
    OBI_SOCKET_CAP_TCP_CONNECT_ADDR = 1ull << 1,
    OBI_SOCKET_CAP_TCP_LISTEN       = 1ull << 2,
    OBI_SOCKET_CAP_CANCEL           = 1ull << 3,
    OBI_SOCKET_CAP_OPTIONS_JSON     = 1ull << 4,
};

typedef struct obi_tcp_connect_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Maximum time to spend connecting; 0 means provider default. */
    uint64_t timeout_ns;

    /* Optional cancellation token (token.api==NULL means "never cancelled"). */
    obi_cancel_token_v0 cancel_token;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_tcp_connect_params_v0;

typedef struct obi_tcp_listen_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Local bind address. */
    obi_socket_addr_v0 bind_addr;

    /* Suggested listen backlog; 0 means provider default. */
    uint32_t backlog;
    uint32_t reserved;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_tcp_listen_params_v0;

typedef struct obi_tcp_listener_v0 obi_tcp_listener_v0;

typedef struct obi_tcp_listener_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Accept an incoming connection.
     *
     * Returns OK and sets out_accepted. If out_accepted is false, no connection was accepted within
     * timeout_ns.
     */
    obi_status (*accept)(void* ctx,
                         uint64_t timeout_ns,
                         obi_cancel_token_v0 cancel_token,
                         obi_reader_v0* out_reader,
                         obi_writer_v0* out_writer,
                         obi_socket_addr_v0* out_remote,
                         bool* out_accepted);

    void (*destroy)(void* ctx);
} obi_tcp_listener_api_v0;

struct obi_tcp_listener_v0 {
    const obi_tcp_listener_api_v0* api;
    void* ctx;
};

typedef struct obi_net_socket_v0 obi_net_socket_v0;

typedef struct obi_net_socket_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Connect to a remote host/port. */
    obi_status (*tcp_connect)(void* ctx,
                              const char* host,
                              uint16_t port,
                              const obi_tcp_connect_params_v0* params,
                              obi_reader_v0* out_reader,
                              obi_writer_v0* out_writer);

    /* Optional: connect to a remote socket address. */
    obi_status (*tcp_connect_addr)(void* ctx,
                                   obi_socket_addr_v0 remote,
                                   const obi_tcp_connect_params_v0* params,
                                   obi_reader_v0* out_reader,
                                   obi_writer_v0* out_writer);

    /* Optional: listen on a local address. */
    obi_status (*tcp_listen)(void* ctx,
                             const obi_tcp_listen_params_v0* params,
                             obi_tcp_listener_v0* out_listener);
} obi_net_socket_api_v0;

struct obi_net_socket_v0 {
    const obi_net_socket_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_SOCKET_V0_H */

