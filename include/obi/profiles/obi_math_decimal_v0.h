/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MATH_DECIMAL_V0_H
#define OBI_PROFILE_MATH_DECIMAL_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MATH_DECIMAL_V0 "obi.profile:math.decimal-0"

typedef uint64_t obi_decimal_ctx_id_v0;
typedef uint64_t obi_decimal_id_v0;

typedef enum obi_decimal_round_v0 {
    OBI_DECIMAL_RND_HALF_EVEN = 0, /* bankers rounding */
    OBI_DECIMAL_RND_HALF_UP   = 1,
    OBI_DECIMAL_RND_HALF_DOWN = 2,
    OBI_DECIMAL_RND_DOWN      = 3, /* toward 0 */
    OBI_DECIMAL_RND_UP        = 4, /* away from 0 */
    OBI_DECIMAL_RND_FLOOR     = 5, /* toward -inf */
    OBI_DECIMAL_RND_CEILING   = 6, /* toward +inf */
} obi_decimal_round_v0;

/* IEEE-754-style condition flags raised by operations. */
enum {
    OBI_DECIMAL_SIG_INEXACT     = 1u << 0,
    OBI_DECIMAL_SIG_ROUNDED     = 1u << 1,
    OBI_DECIMAL_SIG_UNDERFLOW   = 1u << 2,
    OBI_DECIMAL_SIG_OVERFLOW    = 1u << 3,
    OBI_DECIMAL_SIG_SUBNORMAL   = 1u << 4,
    OBI_DECIMAL_SIG_DIV_BY_ZERO = 1u << 5,
    OBI_DECIMAL_SIG_INVALID_OP  = 1u << 6,
    OBI_DECIMAL_SIG_CLAMPED     = 1u << 7,
};

enum {
    OBI_DECIMAL_CAP_QUANTIZE   = 1ull << 0,
    OBI_DECIMAL_CAP_TO_FROM_F64 = 1ull << 1,
};

typedef struct obi_decimal_context_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Precision in decimal digits. */
    uint32_t precision_digits;
    obi_decimal_round_v0 round;

    /* Exponent limits (provider-defined defaults if 0). */
    int32_t emin;
    int32_t emax;

    /* Optional trap mask using OBI_DECIMAL_SIG_* bits (0 means no traps). */
    uint32_t traps;
    uint32_t reserved;
} obi_decimal_context_params_v0;

typedef struct obi_math_decimal_v0 obi_math_decimal_v0;

typedef struct obi_math_decimal_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*ctx_create)(void* ctx,
                             const obi_decimal_context_params_v0* params,
                             obi_decimal_ctx_id_v0* out_ctx);

    void (*ctx_destroy)(void* ctx, obi_decimal_ctx_id_v0 dctx);

    obi_status (*create)(void* ctx, obi_decimal_ctx_id_v0 dctx, obi_decimal_id_v0* out_id);
    void (*destroy)(void* ctx, obi_decimal_id_v0 id);

    obi_status (*copy)(void* ctx, obi_decimal_id_v0 out, obi_decimal_id_v0 src);

    obi_status (*set_i64)(void* ctx, obi_decimal_ctx_id_v0 dctx, obi_decimal_id_v0 id, int64_t v, uint32_t* out_signals);
    obi_status (*set_u64)(void* ctx, obi_decimal_ctx_id_v0 dctx, obi_decimal_id_v0 id, uint64_t v, uint32_t* out_signals);

    /* Parse a decimal string (UTF-8, NUL-terminated). */
    obi_status (*set_str)(void* ctx,
                          obi_decimal_ctx_id_v0 dctx,
                          obi_decimal_id_v0 id,
                          const char* s,
                          uint32_t* out_signals);

    /* Format to a decimal string.
     * If out is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size to the
     * required size including the NUL terminator.
     */
    obi_status (*get_str)(void* ctx,
                          obi_decimal_ctx_id_v0 dctx,
                          obi_decimal_id_v0 id,
                          char* out,
                          size_t out_cap,
                          size_t* out_size);

    /* Compare: out_cmp is -1, 0, or +1. */
    obi_status (*cmp)(void* ctx,
                      obi_decimal_ctx_id_v0 dctx,
                      obi_decimal_id_v0 a,
                      obi_decimal_id_v0 b,
                      int32_t* out_cmp,
                      uint32_t* out_signals);

    obi_status (*add)(void* ctx,
                      obi_decimal_ctx_id_v0 dctx,
                      obi_decimal_id_v0 out,
                      obi_decimal_id_v0 a,
                      obi_decimal_id_v0 b,
                      uint32_t* out_signals);

    obi_status (*sub)(void* ctx,
                      obi_decimal_ctx_id_v0 dctx,
                      obi_decimal_id_v0 out,
                      obi_decimal_id_v0 a,
                      obi_decimal_id_v0 b,
                      uint32_t* out_signals);

    obi_status (*mul)(void* ctx,
                      obi_decimal_ctx_id_v0 dctx,
                      obi_decimal_id_v0 out,
                      obi_decimal_id_v0 a,
                      obi_decimal_id_v0 b,
                      uint32_t* out_signals);

    obi_status (*div)(void* ctx,
                      obi_decimal_ctx_id_v0 dctx,
                      obi_decimal_id_v0 out,
                      obi_decimal_id_v0 a,
                      obi_decimal_id_v0 b,
                      uint32_t* out_signals);

    /* Optional: quantize out to the exponent of 'quant' (mpdecimal-style quantize). */
    obi_status (*quantize)(void* ctx,
                           obi_decimal_ctx_id_v0 dctx,
                           obi_decimal_id_v0 out,
                           obi_decimal_id_v0 a,
                           obi_decimal_id_v0 quant,
                           uint32_t* out_signals);

    /* Optional: convert to/from f64. */
    obi_status (*set_f64)(void* ctx, obi_decimal_ctx_id_v0 dctx, obi_decimal_id_v0 id, double v, uint32_t* out_signals);
    obi_status (*get_f64)(void* ctx, obi_decimal_ctx_id_v0 dctx, obi_decimal_id_v0 id, double* out_v, uint32_t* out_signals);
} obi_math_decimal_api_v0;

struct obi_math_decimal_v0 {
    const obi_math_decimal_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MATH_DECIMAL_V0_H */

