/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_TLS_V0_H
#define OBI_PROFILE_NET_TLS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_NET_TLS_V0 "obi.profile:net.tls-0"

enum {
    OBI_TLS_CAP_ALPN         = 1ull << 0,
    OBI_TLS_CAP_CUSTOM_CA    = 1ull << 1,
    OBI_TLS_CAP_CLIENT_CERT  = 1ull << 2,
    OBI_TLS_CAP_PEER_CERT    = 1ull << 3,
    OBI_TLS_CAP_OPTIONS_JSON = 1ull << 4,
};

typedef struct obi_tls_client_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional SNI / verification name (UTF-8, NUL-terminated). */
    const char* server_name;

    /* 0/1: verify the peer certificate chain. */
    uint8_t verify_peer;
    /* 0/1: verify host name (server_name) against certificate. */
    uint8_t verify_host;
    uint16_t reserved16;

    /* Optional ALPN protocols as a '\n'-separated UTF-8 list (example: "h2\nhttp/1.1"). */
    obi_utf8_view_v0 alpn_protocols;

    /* Optional additional trusted CAs (PEM or DER bundle bytes; provider-defined). */
    obi_bytes_view_v0 ca_bundle;

    /* Optional client certificate/key (PEM or DER bytes; provider-defined). */
    obi_bytes_view_v0 client_cert;
    obi_bytes_view_v0 client_key;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_tls_client_params_v0;

typedef struct obi_tls_session_v0 obi_tls_session_v0;

typedef struct obi_tls_session_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Progress the handshake. Returns OK and sets out_done.
     * Implementations may block for up to timeout_ns, depending on the underlying transport.
     */
    obi_status (*handshake)(void* ctx, uint64_t timeout_ns, bool* out_done);

    /* Read/write application bytes (post-handshake). out_n may be 0 on clean EOF/close. */
    obi_status (*read)(void* ctx, void* dst, size_t dst_cap, size_t* out_n);
    obi_status (*write)(void* ctx, const void* src, size_t src_size, size_t* out_n);

    /* Send a TLS close_notify when supported (optional). */
    obi_status (*shutdown)(void* ctx);

    /* Optional: negotiated ALPN protocol view (empty if none). Valid until session destruction. */
    obi_status (*get_alpn_utf8)(void* ctx, obi_utf8_view_v0* out_proto);

    /* Optional: leaf peer certificate in a provider-defined encoding (commonly PEM). */
    obi_status (*get_peer_cert)(void* ctx, obi_bytes_view_v0* out_cert);

    void (*destroy)(void* ctx);
} obi_tls_session_api_v0;

struct obi_tls_session_v0 {
    const obi_tls_session_api_v0* api;
    void* ctx;
};

typedef struct obi_net_tls_v0 obi_net_tls_v0;

typedef struct obi_net_tls_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create a client TLS session over a caller-owned transport.
     *
     * Lifetime:
     * - transport_reader/transport_writer are caller-owned and MUST remain valid until the TLS
     *   session is destroyed.
     * - The TLS provider MUST NOT destroy the transport reader/writer.
     */
    obi_status (*client_session_create)(void* ctx,
                                        obi_reader_v0 transport_reader,
                                        obi_writer_v0 transport_writer,
                                        const obi_tls_client_params_v0* params,
                                        obi_tls_session_v0* out_session);
} obi_net_tls_api_v0;

struct obi_net_tls_v0 {
    const obi_net_tls_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_TLS_V0_H */

