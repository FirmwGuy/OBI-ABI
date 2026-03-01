/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CRYPTO_RANDOM_V0_H
#define OBI_PROFILE_CRYPTO_RANDOM_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CRYPTO_RANDOM_V0 "obi.profile:crypto.random-0"

typedef struct obi_crypto_random_v0 obi_crypto_random_v0;

typedef struct obi_crypto_random_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Fill dst with cryptographically secure random bytes. */
    obi_status (*fill)(void* ctx, void* dst, size_t dst_size);
} obi_crypto_random_api_v0;

struct obi_crypto_random_v0 {
    const obi_crypto_random_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CRYPTO_RANDOM_V0_H */

