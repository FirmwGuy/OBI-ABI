/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_GEOM_TYPES_V0_H
#define OBI_GEOM_TYPES_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct obi_vec2f_v0 {
    float x;
    float y;
} obi_vec2f_v0;

typedef struct obi_vec3f_v0 {
    float x;
    float y;
    float z;
} obi_vec3f_v0;

typedef struct obi_vec4f_v0 {
    float x;
    float y;
    float z;
    float w;
} obi_vec4f_v0;

typedef struct obi_quatf_v0 {
    float x;
    float y;
    float z;
    float w;
} obi_quatf_v0;

typedef struct obi_rectf_v0 {
    float x;
    float y;
    float w;
    float h;
} obi_rectf_v0;

typedef struct obi_color_rgba8_v0 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} obi_color_rgba8_v0;

typedef struct obi_color_rgba_f32_v0 {
    float r;
    float g;
    float b;
    float a;
} obi_color_rgba_f32_v0;

typedef struct obi_mat4f_v0 {
    /* Column-major 4x4 matrix (OpenGL-style). */
    float m[16];
} obi_mat4f_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_GEOM_TYPES_V0_H */

