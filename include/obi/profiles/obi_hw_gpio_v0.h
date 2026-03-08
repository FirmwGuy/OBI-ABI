/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_HW_GPIO_V0_H
#define OBI_PROFILE_HW_GPIO_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_HW_GPIO_V0 "obi.profile:hw.gpio-0"

enum {
    OBI_GPIO_CAP_EDGE_EVENTS  = 1ull << 0,
    OBI_GPIO_CAP_BIAS         = 1ull << 1,
    OBI_GPIO_CAP_CANCEL       = 1ull << 2,
    OBI_GPIO_CAP_OPTIONS_JSON = 1ull << 3,
};

typedef uint64_t obi_gpio_line_id_v0;

typedef enum obi_gpio_direction_v0 {
    OBI_GPIO_DIR_INPUT  = 0,
    OBI_GPIO_DIR_OUTPUT = 1,
} obi_gpio_direction_v0;

typedef enum obi_gpio_bias_v0 {
    OBI_GPIO_BIAS_DEFAULT   = 0,
    OBI_GPIO_BIAS_DISABLE   = 1,
    OBI_GPIO_BIAS_PULL_UP   = 2,
    OBI_GPIO_BIAS_PULL_DOWN = 3,
} obi_gpio_bias_v0;

enum {
    OBI_GPIO_LINE_ACTIVE_LOW  = 1u << 0,
    OBI_GPIO_LINE_OPEN_DRAIN  = 1u << 1,
    OBI_GPIO_LINE_OPEN_SOURCE = 1u << 2,
};

enum {
    OBI_GPIO_EDGE_FLAG_RISING  = 1u << 0,
    OBI_GPIO_EDGE_FLAG_FALLING = 1u << 1,
};

typedef struct obi_gpio_line_open_params_v0 {
    uint32_t struct_size;
    uint32_t flags;

    obi_gpio_direction_v0 direction;
    int32_t initial_value; /* used for output; ignored for input */

    obi_gpio_bias_v0 bias; /* requires OBI_GPIO_CAP_BIAS when non-default */
    uint32_t edge_flags;   /* OBI_GPIO_EDGE_FLAG_*; requires OBI_GPIO_CAP_EDGE_EVENTS when non-zero */

    /* Optional label (UTF-8, NUL-terminated, borrowed). */
    const char* consumer;

    /* Optional provider-specific JSON object string (borrowed). */
    obi_utf8_view_v0 options_json;
} obi_gpio_line_open_params_v0;

typedef enum obi_gpio_edge_v0 {
    OBI_GPIO_EDGE_NONE    = 0,
    OBI_GPIO_EDGE_RISING  = 1,
    OBI_GPIO_EDGE_FALLING = 2,
} obi_gpio_edge_v0;

typedef struct obi_gpio_event_v0 {
    /* Monotonic timestamp in nanoseconds (0 if unknown). */
    uint64_t mono_ns;

    obi_gpio_edge_v0 edge;
    uint32_t reserved;
} obi_gpio_event_v0;

typedef struct obi_hw_gpio_v0 obi_hw_gpio_v0;

typedef struct obi_hw_gpio_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Open a single GPIO line.
     *
     * chip_path is UTF-8, NUL-terminated (for example "/dev/gpiochip0" on Linux).
     */
    obi_status (*line_open)(void* ctx,
                            const char* chip_path,
                            uint32_t line_offset,
                            const obi_gpio_line_open_params_v0* params,
                            obi_gpio_line_id_v0* out_line);

    void (*line_close)(void* ctx, obi_gpio_line_id_v0 line);

    obi_status (*line_get_value)(void* ctx, obi_gpio_line_id_v0 line, int32_t* out_value);
    obi_status (*line_set_value)(void* ctx, obi_gpio_line_id_v0 line, int32_t value);

    /* Optional edge events: wait for and read the next event.
     *
     * Returns OK and sets out_has_event. If out_has_event is false, no event was observed within
     * timeout_ns.
     *
     * cancel_token.api==NULL means "never cancelled".
     */
    obi_status (*event_next)(void* ctx,
                             obi_gpio_line_id_v0 line,
                             uint64_t timeout_ns,
                             obi_cancel_token_v0 cancel_token,
                             obi_gpio_event_v0* out_event,
                             bool* out_has_event);
} obi_hw_gpio_api_v0;

struct obi_hw_gpio_v0 {
    const obi_hw_gpio_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_HW_GPIO_V0_H */
