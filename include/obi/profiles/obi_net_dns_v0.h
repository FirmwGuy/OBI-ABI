/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_DNS_V0_H
#define OBI_PROFILE_NET_DNS_V0_H

#include "../obi_core_v0.h"
#include "obi_net_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_NET_DNS_V0 "obi.profile:net.dns-0"

enum {
    OBI_DNS_CAP_CANCEL       = 1ull << 0,
    OBI_DNS_CAP_OPTIONS_JSON = 1ull << 1,
};

enum {
    OBI_DNS_RESOLVE_IPV4 = 1u << 0,
    OBI_DNS_RESOLVE_IPV6 = 1u << 1,
};

typedef struct obi_dns_resolve_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Maximum time to spend resolving; 0 means provider default. */
    uint64_t timeout_ns;

    /* Optional cancellation token (token.api==NULL means "never cancelled"). */
    obi_cancel_token_v0 cancel_token;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_dns_resolve_params_v0;

typedef struct obi_net_dns_v0 obi_net_dns_v0;

typedef struct obi_net_dns_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Resolve @name to a list of IP addresses.
     *
     * If out_addrs is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_count to
     * the required number of entries.
     */
    obi_status (*resolve)(void* ctx,
                          const char* name,
                          const obi_dns_resolve_params_v0* params,
                          obi_ip_addr_v0* out_addrs,
                          size_t out_cap,
                          size_t* out_count);
} obi_net_dns_api_v0;

struct obi_net_dns_v0 {
    const obi_net_dns_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_DNS_V0_H */

