/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_GFX_GPU_DEVICE_V0_H
#define OBI_PROFILE_GFX_GPU_DEVICE_V0_H

#include "../obi_core_v0.h"
#include "obi_gfx_types_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_GFX_GPU_DEVICE_V0 "obi.profile:gfx.gpu_device-0"

enum {
    OBI_GPU_CAP_SHADER_GLSL  = 1ull << 0,
    OBI_GPU_CAP_SHADER_SPIRV = 1ull << 1,
    OBI_GPU_CAP_SHADER_WGSL  = 1ull << 2,

    OBI_GPU_CAP_INSTANCING   = 1ull << 8,
    OBI_GPU_CAP_SCISSOR      = 1ull << 9,
    OBI_GPU_CAP_VIEWPORT     = 1ull << 10,
};

typedef uint64_t obi_gpu_buffer_id_v0;
typedef uint64_t obi_gpu_image_id_v0;
typedef uint64_t obi_gpu_sampler_id_v0;
typedef uint64_t obi_gpu_shader_id_v0;
typedef uint64_t obi_gpu_pipeline_id_v0;

typedef enum obi_gpu_buffer_type_v0 {
    OBI_GPU_BUFFER_VERTEX  = 0,
    OBI_GPU_BUFFER_INDEX   = 1,
    OBI_GPU_BUFFER_UNIFORM = 2,
} obi_gpu_buffer_type_v0;

typedef enum obi_gpu_resource_usage_v0 {
    OBI_GPU_USAGE_IMMUTABLE = 0,
    OBI_GPU_USAGE_DYNAMIC   = 1,
    OBI_GPU_USAGE_STREAM    = 2,
} obi_gpu_resource_usage_v0;

typedef enum obi_gpu_index_type_v0 {
    OBI_GPU_INDEX_U16 = 0,
    OBI_GPU_INDEX_U32 = 1,
} obi_gpu_index_type_v0;

typedef enum obi_gpu_primitive_type_v0 {
    OBI_GPU_PRIMITIVE_TRIANGLES = 0,
    OBI_GPU_PRIMITIVE_LINES     = 1,
} obi_gpu_primitive_type_v0;

typedef enum obi_gpu_cull_mode_v0 {
    OBI_GPU_CULL_NONE  = 0,
    OBI_GPU_CULL_BACK  = 1,
    OBI_GPU_CULL_FRONT = 2,
} obi_gpu_cull_mode_v0;

typedef enum obi_gpu_compare_v0 {
    OBI_GPU_CMP_ALWAYS  = 0,
    OBI_GPU_CMP_NEVER   = 1,
    OBI_GPU_CMP_LESS    = 2,
    OBI_GPU_CMP_LEQUAL  = 3,
    OBI_GPU_CMP_GREATER = 4,
    OBI_GPU_CMP_GEQUAL  = 5,
    OBI_GPU_CMP_EQUAL   = 6,
    OBI_GPU_CMP_NEQUAL  = 7,
} obi_gpu_compare_v0;

typedef enum obi_gpu_shader_format_v0 {
    OBI_GPU_SHADER_GLSL  = 0,
    OBI_GPU_SHADER_SPIRV = 1,
    OBI_GPU_SHADER_WGSL  = 2,
} obi_gpu_shader_format_v0;

typedef enum obi_gpu_shader_stage_v0 {
    OBI_GPU_STAGE_VERTEX   = 0,
    OBI_GPU_STAGE_FRAGMENT = 1,
} obi_gpu_shader_stage_v0;

typedef struct obi_gpu_buffer_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint32_t size_bytes;
    uint8_t type;  /* obi_gpu_buffer_type_v0 */
    uint8_t usage; /* obi_gpu_resource_usage_v0 */
    uint16_t reserved16;

    /* Optional initial data (borrowed). If NULL, buffer contents are undefined/zeroed. */
    const void* initial_data;
    uint32_t initial_data_size;
    uint32_t reserved;
} obi_gpu_buffer_desc_v0;

typedef enum obi_gpu_image_format_v0 {
    OBI_GPU_IMAGE_RGBA8 = 0,
} obi_gpu_image_format_v0;

typedef struct obi_gpu_image_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint32_t width;
    uint32_t height;
    uint8_t format; /* obi_gpu_image_format_v0 */
    uint8_t reserved8[3];

    /* Optional initial pixels for RGBA8 (borrowed). */
    const void* initial_pixels;
    uint32_t initial_stride_bytes;
    uint32_t reserved;
} obi_gpu_image_desc_v0;

typedef struct obi_gpu_sampler_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* For v0, sampler fields are provider-defined. A future revision can standardize
     * filtering/wrap modes once multiple providers need the same exact contract.
     */
    obi_utf8_view_v0 options_json;
} obi_gpu_sampler_desc_v0;

typedef struct obi_gpu_shader_stage_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    uint8_t format; /* obi_gpu_shader_format_v0 */
    uint8_t stage;  /* obi_gpu_shader_stage_v0 */
    uint16_t reserved16;

    /* Shader code bytes. For textual formats, this is UTF-8 source code bytes. */
    obi_bytes_view_v0 code;

    /* Optional entrypoint for formats that support it (UTF-8, NUL-terminated). */
    const char* entrypoint;
} obi_gpu_shader_stage_desc_v0;

typedef struct obi_gpu_shader_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_gpu_shader_stage_desc_v0 vs;
    obi_gpu_shader_stage_desc_v0 fs;
} obi_gpu_shader_desc_v0;

typedef enum obi_gpu_vertex_format_v0 {
    OBI_GPU_VERTEX_FLOAT2   = 0,
    OBI_GPU_VERTEX_FLOAT3   = 1,
    OBI_GPU_VERTEX_FLOAT4   = 2,
    OBI_GPU_VERTEX_UBYTE4N  = 3,
} obi_gpu_vertex_format_v0;

typedef struct obi_gpu_vertex_attr_v0 {
    uint8_t location;
    uint8_t format; /* obi_gpu_vertex_format_v0 */
    uint16_t offset_bytes;
} obi_gpu_vertex_attr_v0;

typedef struct obi_gpu_vertex_layout_v0 {
    uint32_t stride_bytes;
    uint32_t attr_count;
    obi_gpu_vertex_attr_v0 attrs[16];
} obi_gpu_vertex_layout_v0;

typedef struct obi_gpu_pipeline_desc_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_gpu_shader_id_v0 shader;
    uint8_t primitive; /* obi_gpu_primitive_type_v0 */
    uint8_t cull;      /* obi_gpu_cull_mode_v0 */
    uint8_t reserved8[6];

    obi_gpu_vertex_layout_v0 vertex_layout;

    /* Depth testing (disabled if compare==ALWAYS and write==0). */
    uint8_t depth_compare; /* obi_gpu_compare_v0 */
    uint8_t depth_write;
    uint16_t reserved16;

    /* Blending (0=disabled, 1=alpha blend). */
    uint8_t blend_enable;
    uint8_t reserved8b[7];
} obi_gpu_pipeline_desc_v0;

typedef struct obi_gpu_bindings_v0 {
    obi_gpu_buffer_id_v0 vertex_buffer;
    uint32_t vertex_offset_bytes;
    uint32_t reserved;

    obi_gpu_buffer_id_v0 index_buffer;
    uint32_t index_offset_bytes;
    uint8_t index_type; /* obi_gpu_index_type_v0 */
    uint8_t reserved8[3];

    /* Fragment images/samplers bound by slot. Slot meanings are shader/provider-defined. */
    obi_gpu_image_id_v0 fs_images[8];
    obi_gpu_sampler_id_v0 fs_samplers[8];
} obi_gpu_bindings_v0;

enum {
    OBI_GPU_CLEAR_COLOR = 1u << 0,
    OBI_GPU_CLEAR_DEPTH = 1u << 1,
};

typedef struct obi_gpu_frame_params_v0 {
    uint32_t struct_size;
    uint32_t clear_flags; /* OBI_GPU_CLEAR_* */

    obi_color_rgba_f32_v0 clear_color;
    float clear_depth;
    float reserved[3];
} obi_gpu_frame_params_v0;

typedef struct obi_gfx_gpu_device_v0 obi_gfx_gpu_device_v0;

typedef struct obi_gfx_gpu_device_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Frame boundaries and presentation (swapchain).
     * @window must refer to a window created by gfx.window_input from the same provider instance,
     * unless the provider explicitly documents a supported cross-provider bridge.
     */
    obi_status (*begin_frame)(void* ctx, obi_window_id_v0 window, const obi_gpu_frame_params_v0* params);
    obi_status (*end_frame)(void* ctx, obi_window_id_v0 window);

    /* Optional viewport/scissor state. */
    obi_status (*set_viewport)(void* ctx, obi_rectf_v0 rect);
    obi_status (*set_scissor)(void* ctx, bool enabled, obi_rectf_v0 rect);

    /* Resource creation and updates. */
    obi_status (*buffer_create)(void* ctx, const obi_gpu_buffer_desc_v0* desc, obi_gpu_buffer_id_v0* out_buf);
    obi_status (*buffer_update)(void* ctx, obi_gpu_buffer_id_v0 buf, uint32_t offset_bytes, obi_bytes_view_v0 data);
    void (*buffer_destroy)(void* ctx, obi_gpu_buffer_id_v0 buf);

    obi_status (*image_create)(void* ctx, const obi_gpu_image_desc_v0* desc, obi_gpu_image_id_v0* out_img);
    obi_status (*image_update_rgba8)(void* ctx,
                                     obi_gpu_image_id_v0 img,
                                     uint32_t x,
                                     uint32_t y,
                                     uint32_t w,
                                     uint32_t h,
                                     const void* pixels,
                                     uint32_t stride_bytes);
    void (*image_destroy)(void* ctx, obi_gpu_image_id_v0 img);

    obi_status (*sampler_create)(void* ctx, const obi_gpu_sampler_desc_v0* desc, obi_gpu_sampler_id_v0* out_samp);
    void (*sampler_destroy)(void* ctx, obi_gpu_sampler_id_v0 samp);

    obi_status (*shader_create)(void* ctx, const obi_gpu_shader_desc_v0* desc, obi_gpu_shader_id_v0* out_shader);
    void (*shader_destroy)(void* ctx, obi_gpu_shader_id_v0 shader);

    obi_status (*pipeline_create)(void* ctx, const obi_gpu_pipeline_desc_v0* desc, obi_gpu_pipeline_id_v0* out_pipe);
    void (*pipeline_destroy)(void* ctx, obi_gpu_pipeline_id_v0 pipe);

    /* Rendering. */
    obi_status (*apply_pipeline)(void* ctx, obi_gpu_pipeline_id_v0 pipe);
    obi_status (*apply_bindings)(void* ctx, const obi_gpu_bindings_v0* bindings);

    /* Apply uniform bytes to a shader stage slot (shader/provider-defined layout). */
    obi_status (*apply_uniforms)(void* ctx,
                                 uint8_t stage /* obi_gpu_shader_stage_v0 */,
                                 uint32_t slot,
                                 obi_bytes_view_v0 bytes);

    /* Draw element_count indices starting at base_element. instance_count may be 1. */
    obi_status (*draw)(void* ctx,
                       uint32_t base_element,
                       uint32_t element_count,
                       uint32_t instance_count);
} obi_gfx_gpu_device_api_v0;

struct obi_gfx_gpu_device_v0 {
    const obi_gfx_gpu_device_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_GFX_GPU_DEVICE_V0_H */
