/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_GFX_WINDOW_INPUT_V0_H
#define OBI_PROFILE_GFX_WINDOW_INPUT_V0_H

#include "../obi_core_v0.h"
#include "obi_gfx_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_GFX_WINDOW_INPUT_V0 "obi.profile:gfx.window_input-0"

enum {
    /* Provider supports wait_event (blocking wait with timeout). */
    OBI_WINDOW_CAP_WAIT_EVENT       = 1ull << 0,
    /* Provider supports clipboard_get_utf8 / clipboard_set_utf8. */
    OBI_WINDOW_CAP_CLIPBOARD_UTF8   = 1ull << 1,
    /* Provider supports start_text_input / stop_text_input. */
    OBI_WINDOW_CAP_TEXT_INPUT       = 1ull << 2,
};

enum {
    OBI_WINDOW_CREATE_RESIZABLE   = 1u << 0,
    OBI_WINDOW_CREATE_HIDDEN      = 1u << 1,
    OBI_WINDOW_CREATE_HIGH_DPI    = 1u << 2,
    OBI_WINDOW_CREATE_BORDERLESS  = 1u << 3,
};

typedef struct obi_window_create_params_v0 {
    const char* title; /* UTF-8, NUL-terminated. */
    uint32_t width;
    uint32_t height;
    uint32_t flags; /* OBI_WINDOW_CREATE_* */
} obi_window_create_params_v0;

typedef enum obi_key_mods_v0 {
    OBI_KEYMOD_SHIFT   = 1u << 0,
    OBI_KEYMOD_CTRL    = 1u << 1,
    OBI_KEYMOD_ALT     = 1u << 2,
    OBI_KEYMOD_SUPER   = 1u << 3,
    OBI_KEYMOD_CAPS    = 1u << 4,
    OBI_KEYMOD_NUM     = 1u << 5,
} obi_key_mods_v0;

/* Keycodes use USB HID Keyboard/Keypad usage IDs (page 0x07).
 * Example: 0x04 = 'A', 0x29 = Escape, 0x2C = Space.
 */
typedef uint16_t obi_keycode_v0;

typedef enum obi_mouse_button_v0 {
    OBI_MOUSE_BUTTON_LEFT   = 1,
    OBI_MOUSE_BUTTON_MIDDLE = 2,
    OBI_MOUSE_BUTTON_RIGHT  = 3,
    OBI_MOUSE_BUTTON_X1     = 4,
    OBI_MOUSE_BUTTON_X2     = 5,
} obi_mouse_button_v0;

typedef enum obi_window_event_type_v0 {
    OBI_WINDOW_EVENT_NONE = 0,

    /* Window lifecycle */
    OBI_WINDOW_EVENT_CLOSE_REQUESTED = 1,
    OBI_WINDOW_EVENT_RESIZED         = 2,

    /* Keyboard */
    OBI_WINDOW_EVENT_KEY             = 10,
    OBI_WINDOW_EVENT_TEXT_INPUT      = 11,

    /* Mouse */
    OBI_WINDOW_EVENT_MOUSE_MOVE      = 20,
    OBI_WINDOW_EVENT_MOUSE_BUTTON    = 21,
    OBI_WINDOW_EVENT_MOUSE_WHEEL     = 22,
} obi_window_event_type_v0;

enum {
    /* Text input events carry up to 31 bytes of UTF-8 plus NUL. Longer text should be split. */
    OBI_WINDOW_TEXT_INPUT_CAP_V0 = 32,
};

typedef struct obi_window_event_v0 {
    obi_window_event_type_v0 type;
    uint32_t reserved;

    /* Monotonic timestamp in nanoseconds (0 if unknown). */
    uint64_t timestamp_ns;

    /* The window associated with the event (0 if not applicable). */
    obi_window_id_v0 window;

    union {
        struct {
            uint32_t width;
            uint32_t height;
        } resized;

        struct {
            obi_keycode_v0 keycode;
            uint16_t reserved16;
            uint32_t mods;      /* obi_key_mods_v0 bitmask */
            uint8_t pressed;    /* 1=down, 0=up */
            uint8_t repeat;     /* 1=repeat, 0=initial */
            uint16_t reserved2;
        } key;

        struct {
            uint32_t size; /* bytes of UTF-8 in text[] excluding trailing NUL */
            char text[OBI_WINDOW_TEXT_INPUT_CAP_V0];
        } text_input;

        struct {
            float x;
            float y;
        } mouse_move;

        struct {
            uint8_t button;  /* obi_mouse_button_v0 */
            uint8_t pressed; /* 1=down, 0=up */
            uint16_t reserved16;
            float x;
            float y;
        } mouse_button;

        struct {
            float dx;
            float dy;
        } mouse_wheel;
    } u;
} obi_window_event_v0;

typedef struct obi_window_input_v0 obi_window_input_v0;

typedef struct obi_window_input_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*create_window)(void* ctx,
                                const obi_window_create_params_v0* params,
                                obi_window_id_v0* out_window);

    void (*destroy_window)(void* ctx, obi_window_id_v0 window);

    /* Poll for the next event. Returns OK and sets out_has_event. */
    obi_status (*poll_event)(void* ctx,
                             obi_window_event_v0* out_event,
                             bool* out_has_event);

    /* Optional blocking wait for an event up to timeout_ns (monotonic). */
    obi_status (*wait_event)(void* ctx,
                             uint64_t timeout_ns,
                             obi_window_event_v0* out_event,
                             bool* out_has_event);

    obi_status (*window_get_framebuffer_size)(void* ctx,
                                              obi_window_id_v0 window,
                                              uint32_t* out_w,
                                              uint32_t* out_h);

    /* Optional: return a content scale factor (for example DPI scaling). */
    obi_status (*window_get_content_scale)(void* ctx,
                                           obi_window_id_v0 window,
                                           float* out_scale_x,
                                           float* out_scale_y);

    /* Clipboard UTF-8 helpers (optional). */
    obi_status (*clipboard_get_utf8)(void* ctx,
                                     char* dst,
                                     size_t dst_cap,
                                     size_t* out_size);

    obi_status (*clipboard_set_utf8)(void* ctx, obi_utf8_view_v0 text);

    /* Text input mode (optional). */
    obi_status (*start_text_input)(void* ctx, obi_window_id_v0 window);
    obi_status (*stop_text_input)(void* ctx, obi_window_id_v0 window);
} obi_window_input_api_v0;

struct obi_window_input_v0 {
    const obi_window_input_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_GFX_WINDOW_INPUT_V0_H */
