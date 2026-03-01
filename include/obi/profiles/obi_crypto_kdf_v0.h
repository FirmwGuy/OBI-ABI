/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CRYPTO_KDF_V0_H
#define OBI_PROFILE_CRYPTO_KDF_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CRYPTO_KDF_V0 "obi.profile:crypto.kdf-0"

enum {
    OBI_KDF_CAP_HKDF         = 1ull << 0,
    OBI_KDF_CAP_PBKDF2       = 1ull << 1,
    OBI_KDF_CAP_ARGON2ID     = 1ull << 2,
    OBI_KDF_CAP_OPTIONS_JSON = 1ull << 3,
};

typedef enum obi_kdf_kind_v0 {
    OBI_KDF_HKDF      = 0,
    OBI_KDF_PBKDF2    = 1,
    OBI_KDF_ARGON2ID  = 2,
} obi_kdf_kind_v0;

typedef struct obi_kdf_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_kdf_kind_v0 kind;
    uint32_t reserved;

    union {
        struct {
            /* Hash algorithm ID string (UTF-8, NUL-terminated), example: "sha256". */
            const char* hash_id;
            obi_bytes_view_v0 salt; /* optional */
            obi_bytes_view_v0 info; /* optional */
        } hkdf;
        struct {
            /* Hash algorithm ID string (UTF-8, NUL-terminated), example: "sha256". */
            const char* hash_id;
            uint32_t iterations;
            uint32_t reserved2;
            obi_bytes_view_v0 salt; /* required */
        } pbkdf2;
        struct {
            uint32_t t_cost;        /* time cost */
            uint32_t m_cost_kib;    /* memory cost in KiB */
            uint32_t parallelism;
            uint32_t reserved2;
            obi_bytes_view_v0 salt; /* required */
        } argon2id;
    } u;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_kdf_params_v0;

typedef struct obi_crypto_kdf_v0 obi_crypto_kdf_v0;

typedef struct obi_crypto_kdf_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Derive output bytes from input.
     *
     * If out_bytes is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*derive_bytes)(void* ctx,
                               obi_bytes_view_v0 input,
                               const obi_kdf_params_v0* params,
                               void* out_bytes,
                               size_t out_cap,
                               size_t* out_size);
} obi_crypto_kdf_api_v0;

struct obi_crypto_kdf_v0 {
    const obi_crypto_kdf_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CRYPTO_KDF_V0_H */

