/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MATH_BIGINT_V0_H
#define OBI_PROFILE_MATH_BIGINT_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MATH_BIGINT_V0 "obi.profile:math.bigint-0"

typedef uint64_t obi_bigint_id_v0;

enum {
    /* Provider supports div_mod. */
    OBI_BIGINT_CAP_DIV_MOD = 1ull << 0,
    /* Provider supports set_str/get_str. */
    OBI_BIGINT_CAP_STRING  = 1ull << 1,
};

typedef struct obi_bigint_bytes_view_v0 {
    /* Big-endian magnitude bytes (no sign). Empty means 0. */
    obi_bytes_view_v0 magnitude_be;
    bool is_negative;
    uint8_t reserved[7];
} obi_bigint_bytes_view_v0;

typedef struct obi_math_bigint_v0 obi_math_bigint_v0;

typedef struct obi_math_bigint_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create a new bigint initialized to 0. */
    obi_status (*create)(void* ctx, obi_bigint_id_v0* out_id);
    void (*destroy)(void* ctx, obi_bigint_id_v0 id);

    /* out = src */
    obi_status (*copy)(void* ctx, obi_bigint_id_v0 out, obi_bigint_id_v0 src);

    obi_status (*set_i64)(void* ctx, obi_bigint_id_v0 id, int64_t v);
    obi_status (*set_u64)(void* ctx, obi_bigint_id_v0 id, uint64_t v);

    /* Set from big-endian magnitude bytes plus sign. */
    obi_status (*set_bytes_be)(void* ctx, obi_bigint_id_v0 id, obi_bigint_bytes_view_v0 bytes);

    /* Get big-endian magnitude bytes plus sign.
     *
     * If out_mag_bytes is NULL or out_mag_cap is too small, return BUFFER_TOO_SMALL and set
     * out_mag_size to the required byte count.
     */
    obi_status (*get_bytes_be)(void* ctx,
                               obi_bigint_id_v0 id,
                               bool* out_is_negative,
                               void* out_mag_bytes,
                               size_t out_mag_cap,
                               size_t* out_mag_size);

    /* Compare: out_cmp is -1, 0, or +1. */
    obi_status (*cmp)(void* ctx, obi_bigint_id_v0 a, obi_bigint_id_v0 b, int32_t* out_cmp);

    obi_status (*add)(void* ctx, obi_bigint_id_v0 out, obi_bigint_id_v0 a, obi_bigint_id_v0 b);
    obi_status (*sub)(void* ctx, obi_bigint_id_v0 out, obi_bigint_id_v0 a, obi_bigint_id_v0 b);
    obi_status (*mul)(void* ctx, obi_bigint_id_v0 out, obi_bigint_id_v0 a, obi_bigint_id_v0 b);

    /* Optional: out_q = a / b, out_r = a % b (truncated division). */
    obi_status (*div_mod)(void* ctx,
                          obi_bigint_id_v0 out_q,
                          obi_bigint_id_v0 out_r,
                          obi_bigint_id_v0 a,
                          obi_bigint_id_v0 b);

    /* Optional: parse from string in base 2..36 (UTF-8, NUL-terminated). */
    obi_status (*set_str)(void* ctx, obi_bigint_id_v0 id, const char* s, uint32_t base);

    /* Optional: format to string in base 2..36.
     * If out is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size to the
     * required size including the NUL terminator.
     */
    obi_status (*get_str)(void* ctx,
                          obi_bigint_id_v0 id,
                          uint32_t base,
                          char* out,
                          size_t out_cap,
                          size_t* out_size);
} obi_math_bigint_api_v0;

struct obi_math_bigint_v0 {
    const obi_math_bigint_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MATH_BIGINT_V0_H */

