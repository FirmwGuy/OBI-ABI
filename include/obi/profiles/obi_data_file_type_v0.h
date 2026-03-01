/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_FILE_TYPE_V0_H
#define OBI_PROFILE_DATA_FILE_TYPE_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_FILE_TYPE_V0 "obi.profile:data.file_type-0"

enum {
    OBI_FILE_TYPE_CAP_FROM_READER  = 1ull << 0,
    OBI_FILE_TYPE_CAP_ENCODING     = 1ull << 1,
    OBI_FILE_TYPE_CAP_CONFIDENCE   = 1ull << 2,
};

typedef struct obi_file_type_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Maximum bytes the provider may read when probing a reader (0 means provider default). */
    uint32_t max_probe_bytes;
    uint32_t reserved;
} obi_file_type_params_v0;

typedef struct obi_file_type_info_v0 {
    /* Provider-owned views. Valid until release() is called. */
    obi_utf8_view_v0 mime_type;
    obi_utf8_view_v0 description;
    obi_utf8_view_v0 encoding; /* may be empty if unknown/unsupported */

    /* Optional 0..100 confidence; 0 if unknown/unsupported. */
    uint32_t confidence;
    uint32_t reserved;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_file_type_info_v0* info);
} obi_file_type_info_v0;

typedef struct obi_data_file_type_v0 obi_data_file_type_v0;

typedef struct obi_data_file_type_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*detect_from_bytes)(void* ctx,
                                    obi_bytes_view_v0 bytes,
                                    const obi_file_type_params_v0* params,
                                    obi_file_type_info_v0* out_info);

    obi_status (*detect_from_reader)(void* ctx,
                                     obi_reader_v0 reader,
                                     const obi_file_type_params_v0* params,
                                     obi_file_type_info_v0* out_info);
} obi_data_file_type_api_v0;

struct obi_data_file_type_v0 {
    const obi_data_file_type_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_FILE_TYPE_V0_H */

