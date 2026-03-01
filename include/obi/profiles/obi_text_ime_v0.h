/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_IME_V0_H
#define OBI_PROFILE_TEXT_IME_V0_H

#include "../obi_core_v0.h"
#include "obi_gfx_types_v0.h"
#include "obi_geom_types_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_IME_V0 "obi.profile:text.ime-0"

enum {
    OBI_IME_CAP_WAIT_EVENT   = 1ull << 0,
    OBI_IME_CAP_CURSOR_RECT  = 1ull << 1,
};

typedef enum obi_ime_event_type_v0 {
    OBI_IME_EVENT_NONE              = 0,
    OBI_IME_EVENT_COMPOSITION_START = 1,
    OBI_IME_EVENT_COMPOSITION_UPDATE = 2,
    OBI_IME_EVENT_COMPOSITION_COMMIT = 3,
    OBI_IME_EVENT_COMPOSITION_END   = 4,
} obi_ime_event_type_v0;

enum {
    /* IME events carry up to 255 bytes of UTF-8 plus NUL. Longer text should be split. */
    OBI_IME_TEXT_CAP_V0 = 256,
};

typedef struct obi_ime_event_v0 {
    obi_ime_event_type_v0 type;
    uint32_t reserved;

    /* Monotonic timestamp in nanoseconds (0 if unknown). */
    uint64_t timestamp_ns;

    /* The window associated with the event (0 if not applicable). */
    obi_window_id_v0 window;

    union {
        struct {
            uint32_t size; /* UTF-8 bytes in text[] excluding trailing NUL */
            uint32_t cursor_byte_offset;
            char text[OBI_IME_TEXT_CAP_V0];
        } composition_update;

        struct {
            uint32_t size; /* UTF-8 bytes in text[] excluding trailing NUL */
            uint32_t reserved;
            char text[OBI_IME_TEXT_CAP_V0];
        } composition_commit;
    } u;
} obi_ime_event_v0;

typedef struct obi_text_ime_v0 obi_text_ime_v0;

typedef struct obi_text_ime_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Enable or disable IME for a window. */
    obi_status (*start)(void* ctx, obi_window_id_v0 window);
    obi_status (*stop)(void* ctx, obi_window_id_v0 window);

    /* Optional: set the IME cursor rectangle in window coordinates (pixels). */
    obi_status (*set_cursor_rect)(void* ctx, obi_window_id_v0 window, obi_rectf_v0 rect);

    /* Poll for the next IME event. Returns OK and sets out_has_event. */
    obi_status (*poll_event)(void* ctx, obi_ime_event_v0* out_event, bool* out_has_event);

    /* Optional blocking wait for an event up to timeout_ns (monotonic). */
    obi_status (*wait_event)(void* ctx,
                             uint64_t timeout_ns,
                             obi_ime_event_v0* out_event,
                             bool* out_has_event);
} obi_text_ime_api_v0;

struct obi_text_ime_v0 {
    const obi_text_ime_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_IME_V0_H */

