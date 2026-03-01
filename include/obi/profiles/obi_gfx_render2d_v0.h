/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_GFX_RENDER2D_V0_H
#define OBI_PROFILE_GFX_RENDER2D_V0_H

#include "../obi_core_v0.h"
#include "obi_gfx_types_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_GFX_RENDER2D_V0 "obi.profile:gfx.render2d-0"

enum {
    OBI_RENDER2D_CAP_SCISSOR = 1ull << 0,
    OBI_RENDER2D_CAP_TEXTURE_RGBA8 = 1ull << 1,
    /* Provider supports begin_frame/end_frame with an OBI window id. */
    OBI_RENDER2D_CAP_WINDOW_TARGET = 1ull << 2,
};

typedef enum obi_blend_mode_v0 {
    OBI_BLEND_ALPHA = 0,
    OBI_BLEND_ADDITIVE = 1,
    OBI_BLEND_MULTIPLY = 2,
} obi_blend_mode_v0;

typedef uint64_t obi_gfx_texture_id_v0;

typedef struct obi_render2d_v0 obi_render2d_v0;

typedef struct obi_render2d_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Optional; if NULL, alpha blending is assumed. */
    obi_status (*set_blend_mode)(void* ctx, obi_blend_mode_v0 mode);

    /* Enable/disable scissor. When enabled, drawing outside rect must be clipped. */
    obi_status (*set_scissor)(void* ctx, bool enabled, obi_rectf_v0 rect);

    obi_status (*draw_rect_filled)(void* ctx, obi_rectf_v0 rect, obi_color_rgba8_v0 color);

    /* Create an RGBA8 texture. If pixels is NULL, the texture is created uninitialized/zeroed. */
    obi_status (*texture_create_rgba8)(void* ctx,
                                       uint32_t width,
                                       uint32_t height,
                                       const void* pixels,
                                       uint32_t stride_bytes,
                                       obi_gfx_texture_id_v0* out_tex);

    /* Update a sub-rectangle of an RGBA8 texture. */
    obi_status (*texture_update_rgba8)(void* ctx,
                                       obi_gfx_texture_id_v0 tex,
                                       uint32_t x,
                                       uint32_t y,
                                       uint32_t w,
                                       uint32_t h,
                                       const void* pixels,
                                       uint32_t stride_bytes);

    void (*texture_destroy)(void* ctx, obi_gfx_texture_id_v0 tex);

    /* Draw a textured quad.
     * - dst is in framebuffer pixels.
     * - uv is normalized [0,1] in texture space.
     */
    obi_status (*draw_texture_quad)(void* ctx,
                                    obi_gfx_texture_id_v0 tex,
                                    obi_rectf_v0 dst,
                                    obi_rectf_v0 uv,
                                    obi_color_rgba8_v0 tint);

    /* Optional: begin/end a frame targeting a specific window. When implemented, hosts should:
     * - call begin_frame(window)
     * - issue draw calls
     * - call end_frame(window) to present
     */
    obi_status (*begin_frame)(void* ctx, obi_window_id_v0 window);
    obi_status (*end_frame)(void* ctx, obi_window_id_v0 window);
} obi_render2d_api_v0;

struct obi_render2d_v0 {
    const obi_render2d_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_GFX_RENDER2D_V0_H */
