/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_MATH_SCIENTIFIC_OPS_V0_H
#define OBI_PROFILE_MATH_SCIENTIFIC_OPS_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_MATH_SCIENTIFIC_OPS_V0 "obi.profile:math.scientific_ops-0"

enum {
    OBI_SCI_CAP_ERF        = 1ull << 0,
    OBI_SCI_CAP_ERFC       = 1ull << 1,
    OBI_SCI_CAP_GAMMA      = 1ull << 2,
    OBI_SCI_CAP_LGAMMA     = 1ull << 3,
    OBI_SCI_CAP_BESSEL_J0  = 1ull << 4,
    OBI_SCI_CAP_BESSEL_J1  = 1ull << 5,
    OBI_SCI_CAP_BESSEL_Y0  = 1ull << 6,
    OBI_SCI_CAP_BESSEL_Y1  = 1ull << 7,
};

typedef struct obi_math_scientific_ops_v0 obi_math_scientific_ops_v0;

typedef struct obi_math_scientific_ops_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*erf)(void* ctx, double x, double* out_y);
    obi_status (*erfc)(void* ctx, double x, double* out_y);

    obi_status (*gamma)(void* ctx, double x, double* out_y);
    obi_status (*lgamma)(void* ctx, double x, double* out_y);

    obi_status (*bessel_j0)(void* ctx, double x, double* out_y);
    obi_status (*bessel_j1)(void* ctx, double x, double* out_y);
    obi_status (*bessel_y0)(void* ctx, double x, double* out_y);
    obi_status (*bessel_y1)(void* ctx, double x, double* out_y);
} obi_math_scientific_ops_api_v0;

struct obi_math_scientific_ops_v0 {
    const obi_math_scientific_ops_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_MATH_SCIENTIFIC_OPS_V0_H */

