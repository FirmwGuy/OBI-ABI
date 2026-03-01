/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_PHYS_WORLD2D_V0_H
#define OBI_PROFILE_PHYS_WORLD2D_V0_H

#include "../obi_core_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_PHYS_WORLD2D_V0 "obi.profile:phys.world2d-0"

enum {
    OBI_PHYS2D_CAP_RAYCAST         = 1ull << 0,
    OBI_PHYS2D_CAP_CONTACT_EVENTS  = 1ull << 1,
};

typedef uint64_t obi_phys2d_body_id_v0;
typedef uint64_t obi_phys2d_collider_id_v0;

typedef enum obi_phys2d_body_type_v0 {
    OBI_PHYS2D_BODY_STATIC    = 0,
    OBI_PHYS2D_BODY_DYNAMIC   = 1,
    OBI_PHYS2D_BODY_KINEMATIC = 2,
} obi_phys2d_body_type_v0;

typedef struct obi_phys2d_world_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_vec2f_v0 gravity;
    uint32_t reserved;
} obi_phys2d_world_params_v0;

typedef struct obi_phys2d_body_def_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_phys2d_body_type_v0 type;
    uint32_t reserved;

    obi_vec2f_v0 position;
    float rotation; /* radians */

    obi_vec2f_v0 linear_velocity;
    float angular_velocity; /* radians/sec */

    float linear_damping;
    float angular_damping;

    uint8_t fixed_rotation;
    uint8_t bullet;
    uint8_t allow_sleep;
    uint8_t reserved8;
} obi_phys2d_body_def_v0;

typedef struct obi_phys2d_collider_common_v0 {
    uint32_t struct_size;
    uint32_t flags;

    float density;
    float friction;
    float restitution;
    uint32_t reserved;

    uint8_t is_sensor;
    uint8_t reserved8[7];
} obi_phys2d_collider_common_v0;

typedef struct obi_phys2d_circle_collider_def_v0 {
    obi_phys2d_collider_common_v0 common;
    obi_vec2f_v0 center;
    float radius;
    float reserved;
} obi_phys2d_circle_collider_def_v0;

typedef struct obi_phys2d_box_collider_def_v0 {
    obi_phys2d_collider_common_v0 common;
    obi_vec2f_v0 center;
    obi_vec2f_v0 half_extents;
    float rotation; /* radians */
    float reserved;
} obi_phys2d_box_collider_def_v0;

typedef struct obi_phys2d_transform_v0 {
    obi_vec2f_v0 position;
    float rotation; /* radians */
    float reserved;
} obi_phys2d_transform_v0;

typedef struct obi_phys2d_raycast_hit_v0 {
    obi_phys2d_body_id_v0 body;
    obi_phys2d_collider_id_v0 collider;

    float fraction; /* 0..1 */
    float reserved;

    obi_vec2f_v0 point;
    obi_vec2f_v0 normal;
} obi_phys2d_raycast_hit_v0;

typedef enum obi_phys2d_contact_kind_v0 {
    OBI_PHYS2D_CONTACT_BEGIN = 1,
    OBI_PHYS2D_CONTACT_END   = 2,
} obi_phys2d_contact_kind_v0;

typedef struct obi_phys2d_contact_event_v0 {
    uint8_t kind; /* obi_phys2d_contact_kind_v0 */
    uint8_t reserved8[7];

    obi_phys2d_body_id_v0 body_a;
    obi_phys2d_body_id_v0 body_b;

    obi_phys2d_collider_id_v0 collider_a;
    obi_phys2d_collider_id_v0 collider_b;
} obi_phys2d_contact_event_v0;

typedef struct obi_phys2d_world_v0 obi_phys2d_world_v0;

typedef struct obi_phys2d_world_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Step the simulation by dt seconds. Iteration counts are provider-defined hints. */
    obi_status (*step)(void* ctx, float dt, uint32_t vel_iters, uint32_t pos_iters);

    obi_status (*body_create)(void* ctx,
                              const obi_phys2d_body_def_v0* def,
                              obi_phys2d_body_id_v0* out_body);

    void (*body_destroy)(void* ctx, obi_phys2d_body_id_v0 body);

    obi_status (*body_get_transform)(void* ctx,
                                     obi_phys2d_body_id_v0 body,
                                     obi_phys2d_transform_v0* out_xf);

    obi_status (*body_set_transform)(void* ctx,
                                     obi_phys2d_body_id_v0 body,
                                     obi_phys2d_transform_v0 xf);

    obi_status (*body_get_linear_velocity)(void* ctx,
                                           obi_phys2d_body_id_v0 body,
                                           obi_vec2f_v0* out_v);

    obi_status (*body_set_linear_velocity)(void* ctx,
                                           obi_phys2d_body_id_v0 body,
                                           obi_vec2f_v0 v);

    obi_status (*body_apply_force_center)(void* ctx,
                                          obi_phys2d_body_id_v0 body,
                                          obi_vec2f_v0 force);

    obi_status (*body_apply_linear_impulse_center)(void* ctx,
                                                   obi_phys2d_body_id_v0 body,
                                                   obi_vec2f_v0 impulse);

    obi_status (*collider_create_circle)(void* ctx,
                                         obi_phys2d_body_id_v0 body,
                                         const obi_phys2d_circle_collider_def_v0* def,
                                         obi_phys2d_collider_id_v0* out_collider);

    obi_status (*collider_create_box)(void* ctx,
                                      obi_phys2d_body_id_v0 body,
                                      const obi_phys2d_box_collider_def_v0* def,
                                      obi_phys2d_collider_id_v0* out_collider);

    void (*collider_destroy)(void* ctx, obi_phys2d_collider_id_v0 collider);

    /* Optional raycast: returns OK and sets out_has_hit. */
    obi_status (*raycast_first)(void* ctx,
                                obi_vec2f_v0 p0,
                                obi_vec2f_v0 p1,
                                obi_phys2d_raycast_hit_v0* out_hit,
                                bool* out_has_hit);

    /* Optional: drain contact events since the last drain. */
    obi_status (*drain_contact_events)(void* ctx,
                                       obi_phys2d_contact_event_v0* events,
                                       size_t event_cap,
                                       size_t* out_event_count);

    void (*destroy)(void* ctx);
} obi_phys2d_world_api_v0;

struct obi_phys2d_world_v0 {
    const obi_phys2d_world_api_v0* api;
    void* ctx;
};

typedef struct obi_phys_world2d_v0 obi_phys_world2d_v0;

typedef struct obi_phys_world2d_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*world_create)(void* ctx,
                              const obi_phys2d_world_params_v0* params,
                              obi_phys2d_world_v0* out_world);
} obi_phys_world2d_api_v0;

struct obi_phys_world2d_v0 {
    const obi_phys_world2d_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_PHYS_WORLD2D_V0_H */

