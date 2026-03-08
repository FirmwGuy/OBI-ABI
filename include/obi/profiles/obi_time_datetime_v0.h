/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TIME_DATETIME_V0_H
#define OBI_PROFILE_TIME_DATETIME_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TIME_DATETIME_V0 "obi.profile:time.datetime-0"

enum {
    /* Provider supports IANA time zones (e.g. "America/Chicago") via OBI_TIME_ZONE_IANA_NAME. */
    OBI_TIME_DATETIME_CAP_TZ_IANA  = 1ull << 0,
    /* Provider supports the local system time zone via OBI_TIME_ZONE_LOCAL. */
    OBI_TIME_DATETIME_CAP_TZ_LOCAL = 1ull << 1,
};

typedef enum obi_time_zone_kind_v0 {
    OBI_TIME_ZONE_UTC                  = 0,
    OBI_TIME_ZONE_FIXED_OFFSET_MINUTES = 1,
    OBI_TIME_ZONE_IANA_NAME            = 2,
    OBI_TIME_ZONE_LOCAL                = 3,
} obi_time_zone_kind_v0;

typedef struct obi_time_zone_spec_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_time_zone_kind_v0 kind;

    /* Used when kind==OBI_TIME_ZONE_FIXED_OFFSET_MINUTES. Minutes east of UTC (for example -360). */
    int32_t offset_minutes;

    /* Used when kind==OBI_TIME_ZONE_IANA_NAME. UTF-8, NUL-terminated, borrowed. */
    const char* iana_name;
} obi_time_zone_spec_v0;

typedef struct obi_time_civil_v0 {
    int32_t year;        /* e.g. 2026 */
    uint8_t month;       /* 1-12 */
    uint8_t day;         /* 1-31 */
    uint8_t hour;        /* 0-23 */
    uint8_t minute;      /* 0-59 */
    uint8_t second;      /* 0-60 (leap seconds when supported) */
    uint8_t reserved;
    uint32_t nanosecond; /* 0-999999999 */
} obi_time_civil_v0;

enum {
    /* If the civil time is ambiguous (DST fall-back), prefer the earlier mapping. */
    OBI_TIME_CIVIL_TO_UNIX_PREFER_EARLIER = 0u,
    /* If the civil time is ambiguous (DST fall-back), prefer the later mapping. */
    OBI_TIME_CIVIL_TO_UNIX_PREFER_LATER   = 1u << 0,
    /* If the civil time is ambiguous or invalid (DST spring-forward gap), fail with ERROR. */
    OBI_TIME_CIVIL_TO_UNIX_REQUIRE_VALID  = 1u << 1,
};

typedef struct obi_time_datetime_v0 obi_time_datetime_v0;

typedef struct obi_time_datetime_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Parse an RFC3339 timestamp (UTF-8, NUL-terminated).
     *
     * Returns OK and writes:
     * - out_unix_ns: nanoseconds since Unix epoch (UTC)
     * - out_offset_minutes: the parsed numeric offset in minutes east of UTC (e.g. -360)
     */
    obi_status (*parse_rfc3339)(void* ctx,
                                const char* s,
                                int64_t* out_unix_ns,
                                int32_t* out_offset_minutes);

    /* Format an RFC3339 timestamp.
     *
     * If out is NULL or out_cap is too small, return BUFFER_TOO_SMALL and set out_size to the
     * required size including the NUL terminator.
     *
     * Providers MUST preserve full nanosecond precision (no rounding) so parse/format can
     * round-trip exactly.
     */
    obi_status (*format_rfc3339)(void* ctx,
                                 int64_t unix_ns,
                                 int32_t offset_minutes,
                                 char* out,
                                 size_t out_cap,
                                 size_t* out_size);

    /* Convert an instant to a civil time in a given zone.
     *
     * For OBI_TIME_ZONE_FIXED_OFFSET_MINUTES, the output offset is the input offset_minutes.
     * For OBI_TIME_ZONE_UTC, the output offset is 0.
     */
    obi_status (*unix_ns_to_civil)(void* ctx,
                                   int64_t unix_ns,
                                   const obi_time_zone_spec_v0* zone,
                                   obi_time_civil_v0* out_civil,
                                   int32_t* out_offset_minutes);

    /* Convert a civil time in a given zone to an instant.
     *
     * flags are OBI_TIME_CIVIL_TO_UNIX_*.
     * out_offset_minutes returns the resolved offset used for the mapping.
     */
    obi_status (*civil_to_unix_ns)(void* ctx,
                                   const obi_time_civil_v0* civil,
                                   const obi_time_zone_spec_v0* zone,
                                   uint32_t flags,
                                   int64_t* out_unix_ns,
                                   int32_t* out_offset_minutes);

    /* Pure arithmetic helpers with overflow checking. */
    obi_status (*add_ns)(void* ctx, int64_t unix_ns, int64_t delta_ns, int64_t* out_unix_ns);
    obi_status (*diff_ns)(void* ctx, int64_t a_unix_ns, int64_t b_unix_ns, int64_t* out_delta_ns);
    obi_status (*cmp)(void* ctx, int64_t a_unix_ns, int64_t b_unix_ns, int32_t* out_cmp);
} obi_time_datetime_api_v0;

struct obi_time_datetime_v0 {
    const obi_time_datetime_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TIME_DATETIME_V0_H */

