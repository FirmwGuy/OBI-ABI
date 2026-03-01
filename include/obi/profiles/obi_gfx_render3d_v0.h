/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_GFX_RENDER3D_V0_H
#define OBI_PROFILE_GFX_RENDER3D_V0_H

#include "../obi_core_v0.h"
#include "obi_gfx_types_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_GFX_RENDER3D_V0 "obi.profile:gfx.render3d-0"

enum {
    OBI_RENDER3D_CAP_INDEXED_MESH   = 1ull << 0,
    OBI_RENDER3D_CAP_TEXTURE_RGBA8  = 1ull << 1,
    OBI_RENDER3D_CAP_DEBUG_LINES    = 1ull << 2,
};

typedef uint64_t obi_gfx3d_mesh_id_v0;
typedef uint64_t obi_gfx3d_texture_id_v0;
typedef uint64_t obi_gfx3d_material_id_v0;

typedef struct obi_gfx3d_mesh_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Vertex attributes. Arrays are borrowed for the duration of mesh_create only. */
    const obi_vec3f_v0* positions;
    const obi_vec3f_v0* normals; /* optional */
    const obi_vec2f_v0* uvs;     /* optional */
    uint32_t vertex_count;

    /* Optional indices (if NULL, provider draws as non-indexed). */
    const uint32_t* indices;
    uint32_t index_count;
} obi_gfx3d_mesh_desc_v0;

typedef struct obi_gfx3d_texture_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint32_t width;
    uint32_t height;

    /* Optional initial pixels (RGBA8). If NULL, texture is uninitialized/zeroed. */
    const void* pixels;
    uint32_t stride_bytes;
    uint32_t reserved;
} obi_gfx3d_texture_desc_v0;

typedef struct obi_gfx3d_material_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_color_rgba_f32_v0 base_color;
    obi_gfx3d_texture_id_v0 base_color_tex; /* 0 means none */
} obi_gfx3d_material_desc_v0;

enum {
    OBI_RENDER3D_CLEAR_COLOR = 1u << 0,
    OBI_RENDER3D_CLEAR_DEPTH = 1u << 1,
};

typedef struct obi_gfx3d_frame_params_v0 {
    uint32_t struct_size;
    uint32_t clear_flags; /* OBI_RENDER3D_CLEAR_* */

    obi_color_rgba_f32_v0 clear_color;
    float clear_depth;
    float reserved[3];
} obi_gfx3d_frame_params_v0;

typedef struct obi_gfx3d_debug_line_v0 {
    obi_vec3f_v0 a;
    obi_vec3f_v0 b;
    obi_color_rgba8_v0 color;
    uint32_t reserved;
} obi_gfx3d_debug_line_v0;

typedef struct obi_render3d_v0 obi_render3d_v0;

typedef struct obi_render3d_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Frame boundaries and presentation. */
    obi_status (*begin_frame)(void* ctx, obi_window_id_v0 window, const obi_gfx3d_frame_params_v0* params);
    obi_status (*end_frame)(void* ctx, obi_window_id_v0 window);

    /* Camera matrices (column-major). */
    obi_status (*set_camera)(void* ctx, obi_mat4f_v0 view, obi_mat4f_v0 proj);

    /* Resources. */
    obi_status (*mesh_create)(void* ctx, const obi_gfx3d_mesh_desc_v0* desc, obi_gfx3d_mesh_id_v0* out_mesh);
    void (*mesh_destroy)(void* ctx, obi_gfx3d_mesh_id_v0 mesh);

    obi_status (*texture_create_rgba8)(void* ctx, const obi_gfx3d_texture_desc_v0* desc, obi_gfx3d_texture_id_v0* out_tex);
    void (*texture_destroy)(void* ctx, obi_gfx3d_texture_id_v0 tex);

    obi_status (*material_create)(void* ctx, const obi_gfx3d_material_desc_v0* desc, obi_gfx3d_material_id_v0* out_mat);
    void (*material_destroy)(void* ctx, obi_gfx3d_material_id_v0 mat);

    /* Draw a mesh with a material and model transform. */
    obi_status (*draw_mesh)(void* ctx,
                            obi_gfx3d_mesh_id_v0 mesh,
                            obi_gfx3d_material_id_v0 mat,
                            obi_mat4f_v0 model);

    /* Optional debug lines. */
    obi_status (*draw_debug_lines)(void* ctx,
                                   const obi_gfx3d_debug_line_v0* lines,
                                   size_t line_count);
} obi_render3d_api_v0;

struct obi_render3d_v0 {
    const obi_render3d_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_GFX_RENDER3D_V0_H */

