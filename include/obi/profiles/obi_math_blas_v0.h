/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MATH_BLAS_V0_H
#define OBI_PROFILE_MATH_BLAS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MATH_BLAS_V0 "obi.profile:math.blas-0"

typedef enum obi_blas_layout_v0 {
    OBI_BLAS_ROW_MAJOR = 101,
    OBI_BLAS_COL_MAJOR = 102,
} obi_blas_layout_v0;

typedef enum obi_blas_transpose_v0 {
    OBI_BLAS_NO_TRANS   = 111,
    OBI_BLAS_TRANS      = 112,
    OBI_BLAS_CONJ_TRANS = 113,
} obi_blas_transpose_v0;

enum {
    OBI_BLAS_CAP_SGEMM = 1ull << 0,
    OBI_BLAS_CAP_DGEMM = 1ull << 1,
};

typedef struct obi_math_blas_v0 obi_math_blas_v0;

typedef struct obi_math_blas_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* CBLAS-style SGEMM:
     *   C = alpha * op(A) * op(B) + beta * C
     */
    obi_status (*sgemm)(void* ctx,
                        obi_blas_layout_v0 layout,
                        obi_blas_transpose_v0 trans_a,
                        obi_blas_transpose_v0 trans_b,
                        int32_t m,
                        int32_t n,
                        int32_t k,
                        float alpha,
                        const float* a,
                        int32_t lda,
                        const float* b,
                        int32_t ldb,
                        float beta,
                        float* c,
                        int32_t ldc);

    /* CBLAS-style DGEMM:
     *   C = alpha * op(A) * op(B) + beta * C
     */
    obi_status (*dgemm)(void* ctx,
                        obi_blas_layout_v0 layout,
                        obi_blas_transpose_v0 trans_a,
                        obi_blas_transpose_v0 trans_b,
                        int32_t m,
                        int32_t n,
                        int32_t k,
                        double alpha,
                        const double* a,
                        int32_t lda,
                        const double* b,
                        int32_t ldb,
                        double beta,
                        double* c,
                        int32_t ldc);
} obi_math_blas_api_v0;

struct obi_math_blas_v0 {
    const obi_math_blas_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MATH_BLAS_V0_H */

