/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CRYPTO_AEAD_V0_H
#define OBI_PROFILE_CRYPTO_AEAD_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CRYPTO_AEAD_V0 "obi.profile:crypto.aead-0"

enum {
    /* Provider supports provider-specific options_json. */
    OBI_AEAD_CAP_OPTIONS_JSON = 1ull << 0,
};

typedef struct obi_aead_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_aead_params_v0;

typedef struct obi_aead_ctx_v0 obi_aead_ctx_v0;

typedef struct obi_aead_ctx_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Encrypt and authenticate.
     *
     * Output format:
     * - out_ciphertext receives ciphertext with the authentication tag appended:
     *   `ciphertext || tag`.
     *
     * If out_ciphertext is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*seal)(void* ctx,
                       obi_bytes_view_v0 nonce,
                       obi_bytes_view_v0 aad,
                       obi_bytes_view_v0 plaintext,
                       void* out_ciphertext,
                       size_t out_cap,
                       size_t* out_size);

    /* Decrypt and authenticate. Returns OK and sets out_ok.
     *
     * Input format:
     * - ciphertext is `ciphertext || tag`.
     *
     * If authentication fails, the provider MUST set out_ok=false and SHOULD NOT write plaintext.
     * If out_plaintext is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*open)(void* ctx,
                       obi_bytes_view_v0 nonce,
                       obi_bytes_view_v0 aad,
                       obi_bytes_view_v0 ciphertext,
                       void* out_plaintext,
                       size_t out_cap,
                       size_t* out_size,
                       bool* out_ok);

    void (*destroy)(void* ctx);
} obi_aead_ctx_api_v0;

struct obi_aead_ctx_v0 {
    const obi_aead_ctx_api_v0* api;
    void* ctx;
};

typedef struct obi_crypto_aead_v0 obi_crypto_aead_v0;

typedef struct obi_crypto_aead_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Query algorithm sizes (bytes). */
    obi_status (*key_size)(void* ctx, const char* algo_id, size_t* out_size);
    obi_status (*nonce_size)(void* ctx, const char* algo_id, size_t* out_size);
    obi_status (*tag_size)(void* ctx, const char* algo_id, size_t* out_size);

    /* Create a new AEAD context for an algorithm ID and key. */
    obi_status (*create)(void* ctx,
                         const char* algo_id,
                         obi_bytes_view_v0 key,
                         const obi_aead_params_v0* params,
                         obi_aead_ctx_v0* out_aead);
} obi_crypto_aead_api_v0;

struct obi_crypto_aead_v0 {
    const obi_crypto_aead_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CRYPTO_AEAD_V0_H */

