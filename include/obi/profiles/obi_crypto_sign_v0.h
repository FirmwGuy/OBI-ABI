/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_CRYPTO_SIGN_V0_H
#define OBI_PROFILE_CRYPTO_SIGN_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_CRYPTO_SIGN_V0 "obi.profile:crypto.sign-0"

enum {
    OBI_SIGN_CAP_KEYGEN       = 1ull << 0,
    OBI_SIGN_CAP_KEY_EXPORT   = 1ull << 1,
    OBI_SIGN_CAP_OPTIONS_JSON = 1ull << 2,
};

typedef struct obi_sign_key_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Key encoding identifier (UTF-8, NUL-terminated), provider-defined.
     * Examples (non-normative): "raw", "pem", "pkcs8_der", "spki_der".
     */
    const char* key_format;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_sign_key_params_v0;

typedef struct obi_sign_public_key_v0 obi_sign_public_key_v0;

typedef struct obi_sign_public_key_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Verify a signature. Returns OK and sets out_ok. */
    obi_status (*verify)(void* ctx,
                         obi_bytes_view_v0 message,
                         obi_bytes_view_v0 signature,
                         bool* out_ok);

    /* Optional: export the key in a requested format.
     * If out_key is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*export_key)(void* ctx,
                             const char* key_format,
                             void* out_key,
                             size_t out_cap,
                             size_t* out_size);

    void (*destroy)(void* ctx);
} obi_sign_public_key_api_v0;

struct obi_sign_public_key_v0 {
    const obi_sign_public_key_api_v0* api;
    void* ctx;
};

typedef struct obi_sign_private_key_v0 obi_sign_private_key_v0;

typedef struct obi_sign_private_key_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Sign a message.
     * If out_sig is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*sign)(void* ctx,
                       obi_bytes_view_v0 message,
                       void* out_sig,
                       size_t out_cap,
                       size_t* out_size);

    /* Optional: export the key in a requested format.
     * If out_key is NULL or too small, return BUFFER_TOO_SMALL and set out_size.
     */
    obi_status (*export_key)(void* ctx,
                             const char* key_format,
                             void* out_key,
                             size_t out_cap,
                             size_t* out_size);

    void (*destroy)(void* ctx);
} obi_sign_private_key_api_v0;

struct obi_sign_private_key_v0 {
    const obi_sign_private_key_api_v0* api;
    void* ctx;
};

typedef struct obi_sign_keygen_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_sign_keygen_params_v0;

typedef struct obi_crypto_sign_v0 obi_crypto_sign_v0;

typedef struct obi_crypto_sign_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*import_public_key)(void* ctx,
                                    const char* algo_id,
                                    obi_bytes_view_v0 key_bytes,
                                    const obi_sign_key_params_v0* params,
                                    obi_sign_public_key_v0* out_key);

    obi_status (*import_private_key)(void* ctx,
                                     const char* algo_id,
                                     obi_bytes_view_v0 key_bytes,
                                     const obi_sign_key_params_v0* params,
                                     obi_sign_private_key_v0* out_key);

    obi_status (*generate_keypair)(void* ctx,
                                   const char* algo_id,
                                   const obi_sign_keygen_params_v0* params,
                                   obi_sign_public_key_v0* out_public_key,
                                   obi_sign_private_key_v0* out_private_key);
} obi_crypto_sign_api_v0;

struct obi_crypto_sign_v0 {
    const obi_crypto_sign_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_CRYPTO_SIGN_V0_H */

