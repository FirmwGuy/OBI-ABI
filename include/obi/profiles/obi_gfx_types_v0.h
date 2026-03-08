/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_GFX_TYPES_V0_H
#define OBI_GFX_TYPES_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Window IDs are produced by obi.profile:gfx.window_input-0 and consumed by other gfx profiles
 * (render2d, gpu_device, render3d, etc.).
 *
 * Window IDs are opaque provider-instance-local handles. Hosts MUST treat them as valid only with
 * gfx profiles obtained from the same provider instance, unless a future profile revision or a
 * provider-documented bridge explicitly states otherwise.
 */
typedef uint64_t obi_window_id_v0;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_GFX_TYPES_V0_H */
