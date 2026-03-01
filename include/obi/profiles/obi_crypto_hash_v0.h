/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CRYPTO_HASH_V0_H
#define OBI_PROFILE_CRYPTO_HASH_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CRYPTO_HASH_V0 "obi.profile:crypto.hash-0"

enum {
    /* Hash contexts support reset(). */
    OBI_HASH_CAP_RESET = 1ull << 0,
};

typedef struct obi_hash_ctx_v0 obi_hash_ctx_v0;

typedef struct obi_hash_ctx_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*update)(void* ctx, obi_bytes_view_v0 bytes);

    /* Finalize into out_digest.
     * If out_digest is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size to
     * the required digest size in bytes.
     */
    obi_status (*final)(void* ctx, void* out_digest, size_t out_cap, size_t* out_size);

    obi_status (*reset)(void* ctx);

    void (*destroy)(void* ctx);
} obi_hash_ctx_api_v0;

struct obi_hash_ctx_v0 {
    const obi_hash_ctx_api_v0* api;
    void* ctx;
};

typedef struct obi_crypto_hash_v0 obi_crypto_hash_v0;

typedef struct obi_crypto_hash_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Query digest size for an algorithm ID (bytes). */
    obi_status (*digest_size)(void* ctx, const char* algo_id, size_t* out_size);

    /* Create a new hashing context for an algorithm ID. */
    obi_status (*create)(void* ctx, const char* algo_id, obi_hash_ctx_v0* out_hash);
} obi_crypto_hash_api_v0;

struct obi_crypto_hash_v0 {
    const obi_crypto_hash_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CRYPTO_HASH_V0_H */

