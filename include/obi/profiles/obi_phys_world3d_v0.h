/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_PHYS_WORLD3D_V0_H
#define OBI_PROFILE_PHYS_WORLD3D_V0_H

#include "../obi_core_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_PHYS_WORLD3D_V0 "obi.profile:phys.world3d-0"

enum {
    OBI_PHYS3D_CAP_RAYCAST         = 1ull << 0,
    OBI_PHYS3D_CAP_CONTACT_EVENTS  = 1ull << 1,
};

typedef uint64_t obi_phys3d_body_id_v0;
typedef uint64_t obi_phys3d_collider_id_v0;

typedef enum obi_phys3d_body_type_v0 {
    OBI_PHYS3D_BODY_STATIC    = 0,
    OBI_PHYS3D_BODY_DYNAMIC   = 1,
    OBI_PHYS3D_BODY_KINEMATIC = 2,
} obi_phys3d_body_type_v0;

typedef struct obi_phys3d_world_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_vec3f_v0 gravity;
    uint32_t reserved;
} obi_phys3d_world_params_v0;

typedef struct obi_phys3d_body_def_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_phys3d_body_type_v0 type;
    uint32_t reserved;

    obi_vec3f_v0 position;
    obi_quatf_v0 rotation;

    obi_vec3f_v0 linear_velocity;
    obi_vec3f_v0 angular_velocity;

    float linear_damping;
    float angular_damping;

    uint8_t allow_sleep;
    uint8_t reserved8[7];
} obi_phys3d_body_def_v0;

typedef struct obi_phys3d_collider_common_v0 {
    uint32_t struct_size;
    uint32_t flags;

    float density;
    float friction;
    float restitution;
    uint32_t reserved;

    uint8_t is_sensor;
    uint8_t reserved8[7];

    /* Local transform relative to the body. */
    obi_vec3f_v0 local_pos;
    obi_quatf_v0 local_rot;
} obi_phys3d_collider_common_v0;

typedef struct obi_phys3d_sphere_collider_def_v0 {
    obi_phys3d_collider_common_v0 common;
    float radius;
    float reserved[3];
} obi_phys3d_sphere_collider_def_v0;

typedef struct obi_phys3d_box_collider_def_v0 {
    obi_phys3d_collider_common_v0 common;
    obi_vec3f_v0 half_extents;
    float reserved;
} obi_phys3d_box_collider_def_v0;

typedef struct obi_phys3d_capsule_collider_def_v0 {
    obi_phys3d_collider_common_v0 common;
    float radius;
    float half_height;
    float reserved[2];
} obi_phys3d_capsule_collider_def_v0;

typedef struct obi_phys3d_transform_v0 {
    obi_vec3f_v0 position;
    float reserved;
    obi_quatf_v0 rotation;
} obi_phys3d_transform_v0;

typedef struct obi_phys3d_raycast_hit_v0 {
    obi_phys3d_body_id_v0 body;
    obi_phys3d_collider_id_v0 collider;

    float fraction; /* 0..1 */
    float reserved[3];

    obi_vec3f_v0 point;
    obi_vec3f_v0 normal;
} obi_phys3d_raycast_hit_v0;

typedef enum obi_phys3d_contact_kind_v0 {
    OBI_PHYS3D_CONTACT_BEGIN = 1,
    OBI_PHYS3D_CONTACT_END   = 2,
} obi_phys3d_contact_kind_v0;

typedef struct obi_phys3d_contact_event_v0 {
    uint8_t kind; /* obi_phys3d_contact_kind_v0 */
    uint8_t reserved8[7];

    obi_phys3d_body_id_v0 body_a;
    obi_phys3d_body_id_v0 body_b;

    obi_phys3d_collider_id_v0 collider_a;
    obi_phys3d_collider_id_v0 collider_b;
} obi_phys3d_contact_event_v0;

typedef struct obi_phys3d_world_v0 obi_phys3d_world_v0;

typedef struct obi_phys3d_world_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Step the simulation by dt seconds. substeps is provider-defined hint (0 means default). */
    obi_status (*step)(void* ctx, float dt, uint32_t substeps);

    obi_status (*body_create)(void* ctx,
                              const obi_phys3d_body_def_v0* def,
                              obi_phys3d_body_id_v0* out_body);

    void (*body_destroy)(void* ctx, obi_phys3d_body_id_v0 body);

    obi_status (*body_get_transform)(void* ctx,
                                     obi_phys3d_body_id_v0 body,
                                     obi_phys3d_transform_v0* out_xf);

    obi_status (*body_set_transform)(void* ctx,
                                     obi_phys3d_body_id_v0 body,
                                     obi_phys3d_transform_v0 xf);

    obi_status (*body_get_linear_velocity)(void* ctx,
                                           obi_phys3d_body_id_v0 body,
                                           obi_vec3f_v0* out_v);

    obi_status (*body_set_linear_velocity)(void* ctx,
                                           obi_phys3d_body_id_v0 body,
                                           obi_vec3f_v0 v);

    obi_status (*body_apply_linear_impulse)(void* ctx,
                                            obi_phys3d_body_id_v0 body,
                                            obi_vec3f_v0 impulse);

    obi_status (*collider_create_sphere)(void* ctx,
                                         obi_phys3d_body_id_v0 body,
                                         const obi_phys3d_sphere_collider_def_v0* def,
                                         obi_phys3d_collider_id_v0* out_collider);

    obi_status (*collider_create_box)(void* ctx,
                                      obi_phys3d_body_id_v0 body,
                                      const obi_phys3d_box_collider_def_v0* def,
                                      obi_phys3d_collider_id_v0* out_collider);

    obi_status (*collider_create_capsule)(void* ctx,
                                          obi_phys3d_body_id_v0 body,
                                          const obi_phys3d_capsule_collider_def_v0* def,
                                          obi_phys3d_collider_id_v0* out_collider);

    void (*collider_destroy)(void* ctx, obi_phys3d_collider_id_v0 collider);

    /* Optional raycast: returns OK and sets out_has_hit. */
    obi_status (*raycast_first)(void* ctx,
                                obi_vec3f_v0 p0,
                                obi_vec3f_v0 p1,
                                obi_phys3d_raycast_hit_v0* out_hit,
                                bool* out_has_hit);

    /* Optional: drain contact events since the last drain. */
    obi_status (*drain_contact_events)(void* ctx,
                                       obi_phys3d_contact_event_v0* events,
                                       size_t event_cap,
                                       size_t* out_event_count);

    void (*destroy)(void* ctx);
} obi_phys3d_world_api_v0;

struct obi_phys3d_world_v0 {
    const obi_phys3d_world_api_v0* api;
    void* ctx;
};

typedef struct obi_phys_world3d_v0 obi_phys_world3d_v0;

typedef struct obi_phys_world3d_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*world_create)(void* ctx,
                              const obi_phys3d_world_params_v0* params,
                              obi_phys3d_world_v0* out_world);
} obi_phys_world3d_api_v0;

struct obi_phys_world3d_v0 {
    const obi_phys_world3d_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_PHYS_WORLD3D_V0_H */

