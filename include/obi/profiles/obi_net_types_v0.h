/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_NET_TYPES_V0_H
#define OBI_PROFILE_NET_TYPES_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum obi_ip_family_v0 {
    OBI_IP_FAMILY_V4 = 4,
    OBI_IP_FAMILY_V6 = 6,
} obi_ip_family_v0;

typedef struct obi_ip_addr_v0 {
    obi_ip_family_v0 family;

    /* For IPv6 link-local addresses; 0 otherwise. */
    uint32_t scope_id;

    /* v4 uses the first 4 bytes; v6 uses all 16. Remaining bytes MUST be set to 0. */
    uint8_t addr[16];
} obi_ip_addr_v0;

typedef struct obi_socket_addr_v0 {
    obi_ip_addr_v0 ip;

    /* Port in host byte order. */
    uint16_t port;
    uint16_t reserved16;
} obi_socket_addr_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_NET_TYPES_V0_H */

