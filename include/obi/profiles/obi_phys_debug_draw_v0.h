/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_PHYS_DEBUG_DRAW_V0_H
#define OBI_PROFILE_PHYS_DEBUG_DRAW_V0_H

#include "../obi_core_v0.h"
#include "obi_geom_types_v0.h"
#include "obi_phys_world2d_v0.h"
#include "obi_phys_world3d_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_PHYS_DEBUG_DRAW_V0 "obi.profile:phys.debug_draw-0"

enum {
    OBI_PHYS_DEBUG_CAP_WORLD2D   = 1ull << 0,
    OBI_PHYS_DEBUG_CAP_WORLD3D   = 1ull << 1,
    OBI_PHYS_DEBUG_CAP_LINES     = 1ull << 2,
    OBI_PHYS_DEBUG_CAP_TRIANGLES = 1ull << 3,
};

enum {
    OBI_PHYS_DEBUG_FLAG_DEFAULT  = 0u,
    OBI_PHYS_DEBUG_FLAG_SHAPES   = 1u << 0,
    OBI_PHYS_DEBUG_FLAG_AABBS    = 1u << 1,
    OBI_PHYS_DEBUG_FLAG_CONTACTS = 1u << 2,
    OBI_PHYS_DEBUG_FLAG_JOINTS   = 1u << 3,
};

typedef struct obi_phys_debug_draw_params_v0 {
    uint32_t struct_size;
    uint32_t flags; /* OBI_PHYS_DEBUG_FLAG_* */
} obi_phys_debug_draw_params_v0;

typedef struct obi_phys_debug_line2d_v0 {
    obi_vec2f_v0 a;
    obi_vec2f_v0 b;
    obi_color_rgba8_v0 color;
} obi_phys_debug_line2d_v0;

typedef struct obi_phys_debug_tri2d_v0 {
    obi_vec2f_v0 p0;
    obi_vec2f_v0 p1;
    obi_vec2f_v0 p2;
    obi_color_rgba8_v0 color;
} obi_phys_debug_tri2d_v0;

typedef struct obi_phys_debug_line3d_v0 {
    obi_vec3f_v0 a;
    obi_vec3f_v0 b;
    obi_color_rgba8_v0 color;
    uint32_t reserved;
} obi_phys_debug_line3d_v0;

typedef struct obi_phys_debug_tri3d_v0 {
    obi_vec3f_v0 p0;
    obi_vec3f_v0 p1;
    obi_vec3f_v0 p2;
    obi_color_rgba8_v0 color;
    uint32_t reserved;
} obi_phys_debug_tri3d_v0;

typedef struct obi_phys_debug_draw_v0 obi_phys_debug_draw_v0;

typedef struct obi_phys_debug_draw_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Collect debug primitives for a 2D world. */
    obi_status (*collect_world2d)(void* ctx,
                                  const obi_phys2d_world_v0* world,
                                  const obi_phys_debug_draw_params_v0* params,
                                  obi_phys_debug_line2d_v0* lines,
                                  size_t line_cap,
                                  size_t* out_line_count,
                                  obi_phys_debug_tri2d_v0* tris,
                                  size_t tri_cap,
                                  size_t* out_tri_count);

    /* Collect debug primitives for a 3D world. */
    obi_status (*collect_world3d)(void* ctx,
                                  const obi_phys3d_world_v0* world,
                                  const obi_phys_debug_draw_params_v0* params,
                                  obi_phys_debug_line3d_v0* lines,
                                  size_t line_cap,
                                  size_t* out_line_count,
                                  obi_phys_debug_tri3d_v0* tris,
                                  size_t tri_cap,
                                  size_t* out_tri_count);
} obi_phys_debug_draw_api_v0;

struct obi_phys_debug_draw_v0 {
    const obi_phys_debug_draw_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_PHYS_DEBUG_DRAW_V0_H */

