/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_DATA_COMPRESSION_V0_H
#define OBI_PROFILE_DATA_COMPRESSION_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_DATA_COMPRESSION_V0 "obi.profile:data.compression-0"

enum {
    /* Provider supports dictionaries (obi_compression_params_v0.dictionary). */
    OBI_COMPRESSION_CAP_DICTIONARY   = 1ull << 0,
    /* Provider supports provider-specific options_json. */
    OBI_COMPRESSION_CAP_OPTIONS_JSON = 1ull << 1,
};

typedef struct obi_compression_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    /* Compression level. Interpretation is codec/provider-defined.
     * -1 means provider default.
     */
    int32_t level;
    uint32_t reserved;

    /* Optional dictionary bytes (borrowed for duration of the call). */
    obi_bytes_view_v0 dictionary;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_compression_params_v0;

typedef struct obi_data_compression_v0 obi_data_compression_v0;

typedef struct obi_data_compression_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Compress bytes read from src into dst. */
    obi_status (*compress)(void* ctx,
                           const char* codec_id,
                           const obi_compression_params_v0* params,
                           obi_reader_v0 src,
                           obi_writer_v0 dst,
                           uint64_t* out_bytes_in,
                           uint64_t* out_bytes_out);

    /* Decompress bytes read from src into dst. */
    obi_status (*decompress)(void* ctx,
                             const char* codec_id,
                             const obi_compression_params_v0* params,
                             obi_reader_v0 src,
                             obi_writer_v0 dst,
                             uint64_t* out_bytes_in,
                             uint64_t* out_bytes_out);
} obi_data_compression_api_v0;

struct obi_data_compression_v0 {
    const obi_data_compression_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_DATA_COMPRESSION_V0_H */

