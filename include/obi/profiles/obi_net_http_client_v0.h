/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_HTTP_CLIENT_V0_H
#define OBI_PROFILE_NET_HTTP_CLIENT_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_NET_HTTP_CLIENT_V0 "obi.profile:net.http_client-0"

enum {
    /* Provider supports request_async and returns jobs that can be polled. */
    OBI_HTTP_CAP_ASYNC = 1ull << 0,
    /* Async requests progress only via a pump profile (no internal threads). */
    OBI_HTTP_CAP_REQUIRES_PUMP = 1ull << 1,
    /* Provider supports request_ex / request_async_ex with extended request options. */
    OBI_HTTP_CAP_REQUEST_EX = 1ull << 2,
    /* Provider supports async requests with BODY_READER (streaming upload). */
    OBI_HTTP_CAP_ASYNC_BODY_READER = 1ull << 3,
};

typedef struct obi_http_header_kv_v0 {
    const char* key;
    const char* value;
} obi_http_header_kv_v0;

typedef struct obi_http_headers_view_v0 {
    const obi_http_header_kv_v0* items;
    size_t count;
} obi_http_headers_view_v0;

typedef enum obi_http_body_kind_v0 {
    OBI_HTTP_BODY_NONE  = 0,
    OBI_HTTP_BODY_BYTES = 1,
    OBI_HTTP_BODY_READER = 2,
} obi_http_body_kind_v0;

typedef struct obi_http_body_v0 {
    obi_http_body_kind_v0 kind;
    uint32_t reserved;
    union {
        struct {
            const void* bytes;
            size_t size;
        } as_bytes;
        obi_reader_v0 as_reader;
    } u;
} obi_http_body_v0;

typedef struct obi_http_request_v0 {
    /* Optional; defaults to "GET" when NULL. */
    const char* method;
    /* Required. */
    const char* url;
    /* Optional. */
    obi_http_headers_view_v0 headers;
    /* Optional. */
    obi_http_body_v0 body;
} obi_http_request_v0;

typedef struct obi_http_request_ex_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional; defaults to "GET" when NULL. */
    const char* method;
    /* Required. */
    const char* url;
    /* Optional. */
    obi_http_headers_view_v0 headers;
    /* Optional. */
    obi_http_body_v0 body;

    /* Request options (0/NULL means provider default). */
    uint64_t timeout_ns;

    uint32_t max_redirects;
    uint8_t  follow_redirects;
    uint8_t  tls_verify_peer;
    uint8_t  tls_verify_host;
    uint8_t  reserved8;

    /* Optional, UTF-8 NUL-terminated user agent string. */
    const char* user_agent;
} obi_http_request_ex_v0;

typedef struct obi_http_response_v0 {
    int32_t status_code;
    uint32_t reserved;

    /* Provider-owned header views. Valid until response_release is called. */
    obi_http_headers_view_v0 headers;

    /* Provider-owned body reader. Valid until response_release is called.
     * If body.api == NULL, there is no response body.
     */
    obi_reader_v0 body;

    /* Provider-owned response state, released by response_release. */
    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_http_response_v0* resp);
} obi_http_response_v0;

typedef struct obi_http_job_v0 obi_http_job_v0;

typedef struct obi_http_job_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Poll for completion. Returns OK and sets out_done. */
    obi_status (*poll)(void* ctx, uint64_t timeout_ns, bool* out_done);

    /* When complete, transfer ownership of the response into out_resp. */
    obi_status (*take_response)(void* ctx, obi_http_response_v0* out_resp);

    void (*cancel)(void* ctx);
    void (*destroy)(void* ctx);
} obi_http_job_api_v0;

struct obi_http_job_v0 {
    const obi_http_job_api_v0* api;
    void* ctx;
};

typedef struct obi_http_client_v0 obi_http_client_v0;

typedef struct obi_http_client_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Synchronous request/response. The provider must not retain pointers to req fields after returning. */
    obi_status (*request)(void* ctx,
                          const obi_http_request_v0* req,
                          obi_http_response_v0* out_resp);

    /* Optional async request. If implemented, the provider must copy any data it needs from req
     * before returning (including headers/body bytes). No req pointers may be retained.
     */
    obi_status (*request_async)(void* ctx,
                                const obi_http_request_v0* req,
                                obi_http_job_v0* out_job);

    /* Extended request entrypoints (optional). */
    obi_status (*request_ex)(void* ctx,
                             const obi_http_request_ex_v0* req,
                             obi_http_response_v0* out_resp);

    obi_status (*request_async_ex)(void* ctx,
                                   const obi_http_request_ex_v0* req,
                                   obi_http_job_v0* out_job);
} obi_http_client_api_v0;

struct obi_http_client_v0 {
    const obi_http_client_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_HTTP_CLIENT_V0_H */
