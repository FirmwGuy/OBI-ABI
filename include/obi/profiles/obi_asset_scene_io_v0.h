/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_ASSET_SCENE_IO_V0_H
#define OBI_PROFILE_ASSET_SCENE_IO_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_ASSET_SCENE_IO_V0 "obi.profile:asset.scene_io-0"

enum {
    OBI_SCENE_IO_CAP_OPEN_BYTES   = 1ull << 0,
    OBI_SCENE_IO_CAP_EXPORT_WRITE = 1ull << 1,
};

typedef struct obi_scene_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Optional format hint (UTF-8, NUL-terminated), e.g. "gltf", "glb". */
    const char* format_hint;

    /* Optional provider-specific JSON options object (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_scene_open_params_v0;

typedef struct obi_scene_blob_info_v0 {
    /* Provider-owned views, valid until next blob_info call or asset destruction. */
    obi_utf8_view_v0 name;
    obi_utf8_view_v0 mime_type;

    /* Size when known; 0 if unknown. */
    uint64_t size_bytes;
} obi_scene_blob_info_v0;

typedef struct obi_scene_asset_v0 obi_scene_asset_v0;

typedef struct obi_scene_asset_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Provider-owned scene JSON view (UTF-8). Valid until asset destruction. */
    obi_status (*get_scene_json)(void* ctx, obi_utf8_view_v0* out_scene_json);

    obi_status (*blob_count)(void* ctx, uint32_t* out_count);
    obi_status (*blob_info)(void* ctx, uint32_t blob_index, obi_scene_blob_info_v0* out_info);

    /* Open a provider-owned reader for the blob payload. */
    obi_status (*open_blob_reader)(void* ctx, uint32_t blob_index, obi_reader_v0* out_reader);

    void (*destroy)(void* ctx);
} obi_scene_asset_api_v0;

struct obi_scene_asset_v0 {
    const obi_scene_asset_api_v0* api;
    void* ctx;
};

typedef struct obi_asset_scene_io_v0 obi_asset_scene_io_v0;

typedef struct obi_asset_scene_io_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*open_reader)(void* ctx,
                              obi_reader_v0 reader,
                              const obi_scene_open_params_v0* params,
                              obi_scene_asset_v0* out_asset);

    obi_status (*open_bytes)(void* ctx,
                             obi_bytes_view_v0 bytes,
                             const obi_scene_open_params_v0* params,
                             obi_scene_asset_v0* out_asset);

    /* Optional: export the scene asset to a writer in a requested format (codec_id). */
    obi_status (*export_to_writer)(void* ctx,
                                   const char* codec_id,
                                   const obi_scene_open_params_v0* params,
                                   const obi_scene_asset_v0* asset,
                                   obi_writer_v0 out_bytes,
                                   uint64_t* out_bytes_written);
} obi_asset_scene_io_api_v0;

struct obi_asset_scene_io_v0 {
    const obi_asset_scene_io_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_ASSET_SCENE_IO_V0_H */

