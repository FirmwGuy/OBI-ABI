/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026-present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_LEGAL_V0_H
#define OBI_LEGAL_V0_H

#include "obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_LEGAL_ABI_MAJOR 0u
#define OBI_LEGAL_ABI_MINOR 1u

/* Coarse copyleft severity for policy. This is independent from patent posture. */
typedef enum obi_legal_copyleft_class_v0 {
    OBI_LEGAL_COPYLEFT_UNKNOWN    = 0,
    OBI_LEGAL_COPYLEFT_PERMISSIVE = 1,
    OBI_LEGAL_COPYLEFT_WEAK       = 2,
    OBI_LEGAL_COPYLEFT_STRONG     = 3,
} obi_legal_copyleft_class_v0;

/* High-level patent posture signal for policy. This is not legal advice. */
typedef enum obi_legal_patent_posture_v0 {
    OBI_LEGAL_PATENT_POSTURE_UNKNOWN        = 0,
    OBI_LEGAL_PATENT_POSTURE_ORDINARY       = 1,
    OBI_LEGAL_PATENT_POSTURE_EXPLICIT_GRANT = 2,
    OBI_LEGAL_PATENT_POSTURE_SENSITIVE      = 3,
    OBI_LEGAL_PATENT_POSTURE_RESTRICTED     = 4,
} obi_legal_patent_posture_v0;

typedef enum obi_legal_dependency_relation_v0 {
    OBI_LEGAL_DEP_REQUIRED_BUILD   = 0,
    OBI_LEGAL_DEP_REQUIRED_RUNTIME = 1,
    OBI_LEGAL_DEP_OPTIONAL_RUNTIME = 2,
    OBI_LEGAL_DEP_ROUTE_SCOPED     = 3,
} obi_legal_dependency_relation_v0;

typedef enum obi_legal_route_availability_v0 {
    OBI_LEGAL_ROUTE_AVAILABILITY_UNKNOWN                   = 0,
    OBI_LEGAL_ROUTE_AVAILABILITY_AVAILABLE                 = 1,
    OBI_LEGAL_ROUTE_AVAILABILITY_UNAVAILABLE               = 2,
    OBI_LEGAL_ROUTE_AVAILABILITY_DISABLED_AT_BUILD         = 3,
    OBI_LEGAL_ROUTE_AVAILABILITY_MISSING_RUNTIME_COMPONENT = 4,
} obi_legal_route_availability_v0;

typedef enum obi_legal_preset_v0 {
    OBI_LEGAL_PRESET_CUSTOM               = 0,
    OBI_LEGAL_PRESET_PERMISSIVE_ONLY      = 1,
    OBI_LEGAL_PRESET_UP_TO_WEAK_COPYLEFT  = 2,
    OBI_LEGAL_PRESET_UP_TO_STRONG_COPYLEFT = 3,
} obi_legal_preset_v0;

typedef enum obi_legal_plan_status_v0 {
    OBI_LEGAL_PLAN_STATUS_BLOCKED    = 0,
    OBI_LEGAL_PLAN_STATUS_SELECTABLE = 1,
} obi_legal_plan_status_v0;

enum {
    /* Metadata was inferred or conservatively collapsed rather than proven per route. */
    OBI_LEGAL_TERM_FLAG_CONSERVATIVE = 1ull << 0,
};

enum {
    /* Provider has route-sensitive legal behavior; hosts should prefer routes over provider-wide facts. */
    OBI_PROVIDER_LEGAL_META_FLAG_ROUTE_SENSITIVE                 = 1ull << 0,
    /* Provider may still activate unknown runtime components beyond the declared dependency closure. */
    OBI_PROVIDER_LEGAL_META_FLAG_UNKNOWN_RUNTIME_COMPONENTS_POSSIBLE = 1ull << 1,
};

enum {
    /* Provider may choose this route when the host does not request a narrower route selector. */
    OBI_LEGAL_ROUTE_FLAG_DEFAULT = 1ull << 0,
};

enum {
    /* Unknown copyleft class is treated as acceptable. */
    OBI_LEGAL_SELECTOR_POLICY_FLAG_ALLOW_UNKNOWN_COPYLEFT = 1ull << 0,
    /* Unknown patent posture is treated as acceptable. */
    OBI_LEGAL_SELECTOR_POLICY_FLAG_ALLOW_UNKNOWN_PATENT_POSTURE = 1ull << 1,
    /* Routes not available on the current machine may still be considered reportable candidates. */
    OBI_LEGAL_SELECTOR_POLICY_FLAG_ALLOW_UNAVAILABLE_ROUTES = 1ull << 2,
    /* Optional runtime components are acceptable when otherwise policy-compliant. */
    OBI_LEGAL_SELECTOR_POLICY_FLAG_ALLOW_OPTIONAL_RUNTIME_COMPONENTS = 1ull << 3,
};

#define OBI_LEGAL_PATENT_POSTURE_MASK(v) (1ull << (uint32_t)(v))

enum {
    OBI_LEGAL_PATENT_MASK_ALL =
        OBI_LEGAL_PATENT_POSTURE_MASK(OBI_LEGAL_PATENT_POSTURE_UNKNOWN) |
        OBI_LEGAL_PATENT_POSTURE_MASK(OBI_LEGAL_PATENT_POSTURE_ORDINARY) |
        OBI_LEGAL_PATENT_POSTURE_MASK(OBI_LEGAL_PATENT_POSTURE_EXPLICIT_GRANT) |
        OBI_LEGAL_PATENT_POSTURE_MASK(OBI_LEGAL_PATENT_POSTURE_SENSITIVE) |
        OBI_LEGAL_PATENT_POSTURE_MASK(OBI_LEGAL_PATENT_POSTURE_RESTRICTED),
};

typedef struct obi_legal_term_v0 {
    uint32_t struct_size;
    uint32_t copyleft_class; /* obi_legal_copyleft_class_v0 */
    uint32_t patent_posture; /* obi_legal_patent_posture_v0 */
    uint32_t reserved;
    uint64_t flags;

    /* SPDX license expression for the described legal fact. Optional but strongly recommended. */
    const char* spdx_expression;

    /* Optional short UTF-8 note explaining why this fact is conservative/conditional. */
    const char* summary_utf8;
} obi_legal_term_v0;

typedef struct obi_legal_selector_term_v0 {
    uint32_t struct_size;
    uint32_t reserved;

    /* Generic selector pair used to narrow a route, e.g. key="codec", value="h264". */
    const char* key_utf8;
    const char* value_utf8;
} obi_legal_selector_term_v0;

typedef struct obi_legal_dependency_v0 {
    uint32_t struct_size;
    uint32_t relation; /* obi_legal_dependency_relation_v0 */
    uint32_t reserved;
    uint32_t reserved2;
    uint64_t flags;

    /* Stable provider-local identifier for cross-reference from routes. */
    const char* dependency_id;
    const char* name;
    const char* version;

    obi_legal_term_v0 legal;
} obi_legal_dependency_v0;

typedef struct obi_legal_route_v0 {
    uint32_t struct_size;
    uint32_t availability; /* obi_legal_route_availability_v0 */
    uint32_t reserved;
    uint32_t reserved2;
    uint64_t flags;

    /* Stable provider-local route identifier, e.g. "decode:h264:gst-libav". */
    const char* route_id;
    const char* profile_id;
    const char* summary_utf8;
    const char* implementation_utf8;

    const obi_legal_selector_term_v0* selectors;
    size_t selector_count;

    /* References into obi_provider_legal_metadata_v0.dependencies by dependency_id. */
    const char* const* dependency_ids;
    size_t dependency_id_count;

    /* Effective legal posture of this specific route. */
    obi_legal_term_v0 effective_license;
} obi_legal_route_v0;

struct obi_provider_legal_metadata_v0 {
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t flags;

    /* License of the provider module itself as shipped. */
    obi_legal_term_v0 module_license;

    /* Effective legal posture when the host selects this provider without a narrower route.
     * Providers with materially route-sensitive behavior should set this conservatively or unknown
     * and expose precise facts via routes[].
     */
    obi_legal_term_v0 effective_license;

    const obi_legal_dependency_v0* dependencies;
    size_t dependency_count;

    const obi_legal_route_v0* routes;
    size_t route_count;
};

typedef struct obi_legal_selector_policy_v0 {
    uint32_t struct_size;
    uint32_t preset; /* obi_legal_preset_v0 */
    uint32_t max_copyleft_class; /* obi_legal_copyleft_class_v0 */
    uint32_t reserved;
    uint64_t flags;

    /* Bitmask of allowed patent postures (use OBI_LEGAL_PATENT_POSTURE_MASK(...)). */
    uint64_t allowed_patent_postures;
} obi_legal_selector_policy_v0;

typedef struct obi_legal_requirement_v0 {
    uint32_t struct_size;
    uint32_t reserved;

    /* Required profile to satisfy under the legal policy. */
    const char* profile_id;

    /* Optional generic selectors to request a narrower route, e.g. codec/container/backend family. */
    const obi_legal_selector_term_v0* selectors;
    size_t selector_count;
} obi_legal_requirement_v0;

typedef struct obi_legal_plan_item_v0 {
    uint32_t struct_size;
    uint32_t status; /* obi_legal_plan_status_v0 */
    uint32_t requirement_index;
    uint32_t reserved;

    const char* profile_id;
    const char* provider_id; /* NULL when blocked */
    const char* route_id;    /* Optional when selection is provider-wide */
    const char* reason_utf8; /* Optional explanation for blocked/degraded results */
} obi_legal_plan_item_v0;

typedef struct obi_legal_plan_v0 {
    uint32_t struct_size;
    uint32_t overall_status; /* obi_legal_plan_status_v0 */
    uint32_t preset;         /* obi_legal_preset_v0 */
    uint32_t reserved;
    uint64_t flags;

    const obi_legal_plan_item_v0* items;
    size_t item_count;
} obi_legal_plan_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_LEGAL_V0_H */
