/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_ASSET_MESH_IO_V0_H
#define OBI_PROFILE_ASSET_MESH_IO_V0_H

#include "../obi_core_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_ASSET_MESH_IO_V0 "obi.profile:asset.mesh_io-0"

enum {
    OBI_MESH_IO_CAP_OPEN_BYTES  = 1ull << 0,
    OBI_MESH_IO_CAP_NORMALS     = 1ull << 1,
    OBI_MESH_IO_CAP_UVS         = 1ull << 2,
    OBI_MESH_IO_CAP_INDICES     = 1ull << 3,
};

typedef struct obi_mesh_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "obj", "gltf", "glb". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_mesh_open_params_v0;

typedef struct obi_mesh_info_v0 {
    /* Provider-owned name view, valid until next mesh_info call or asset destruction. */
    obi_utf8_view_v0 name;

    uint32_t vertex_count;
    uint32_t index_count;

    uint8_t has_normals;
    uint8_t has_uvs;
    uint8_t has_indices;
    uint8_t reserved8;
} obi_mesh_info_v0;

typedef struct obi_mesh_asset_v0 obi_mesh_asset_v0;

typedef struct obi_mesh_asset_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*mesh_count)(void* ctx, uint32_t* out_count);

    obi_status (*mesh_info)(void* ctx, uint32_t mesh_index, obi_mesh_info_v0* out_info);

    obi_status (*mesh_get_positions)(void* ctx,
                                     uint32_t mesh_index,
                                     obi_vec3f_v0* positions,
                                     size_t pos_cap,
                                     size_t* out_pos_count);

    obi_status (*mesh_get_normals)(void* ctx,
                                   uint32_t mesh_index,
                                   obi_vec3f_v0* normals,
                                   size_t n_cap,
                                   size_t* out_n_count);

    obi_status (*mesh_get_uvs)(void* ctx,
                               uint32_t mesh_index,
                               obi_vec2f_v0* uvs,
                               size_t uv_cap,
                               size_t* out_uv_count);

    obi_status (*mesh_get_indices_u32)(void* ctx,
                                       uint32_t mesh_index,
                                       uint32_t* indices,
                                       size_t idx_cap,
                                       size_t* out_idx_count);

    void (*destroy)(void* ctx);
} obi_mesh_asset_api_v0;

struct obi_mesh_asset_v0 {
    const obi_mesh_asset_api_v0* api;
    void* ctx;
};

typedef struct obi_asset_mesh_io_v0 obi_asset_mesh_io_v0;

typedef struct obi_asset_mesh_io_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_mesh_open_params_v0* params,
                              obi_mesh_asset_v0* out_asset);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_mesh_open_params_v0* params,
                             obi_mesh_asset_v0* out_asset);
} obi_asset_mesh_io_api_v0;

struct obi_asset_mesh_io_v0 {
    const obi_asset_mesh_io_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_ASSET_MESH_IO_V0_H */

