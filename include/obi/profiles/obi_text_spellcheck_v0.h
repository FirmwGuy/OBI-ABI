/* SPDX-License-Identifier: MPL-2.0 */
/* SPDX-FileCopyrightText: © 2026–present Victor M. Barrientos <firmw.guy@gmail.com> */

#ifndef OBI_PROFILE_TEXT_SPELLCHECK_V0_H
#define OBI_PROFILE_TEXT_SPELLCHECK_V0_H

#include "../obi_core_v0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBI_PROFILE_TEXT_SPELLCHECK_V0 "obi.profile:text.spellcheck-0"

enum {
    OBI_SPELL_CAP_PERSONAL_DICT = 1ull << 0,
};

typedef struct obi_spell_suggestions_v0 {
    /* Provider-owned UTF-8 views. Valid until release(). */
    const obi_utf8_view_v0* items;
    size_t count;

    void* release_ctx;
    void (*release)(void* release_ctx, struct obi_spell_suggestions_v0* sug);
} obi_spell_suggestions_v0;

typedef struct obi_spell_session_v0 obi_spell_session_v0;

typedef struct obi_spell_session_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    obi_status (*check_word_utf8)(void* ctx, obi_utf8_view_v0 word, bool* out_correct);

    obi_status (*suggest_utf8)(void* ctx, obi_utf8_view_v0 word, obi_spell_suggestions_v0* out_suggestions);

    obi_status (*personal_add_utf8)(void* ctx, obi_utf8_view_v0 word);
    obi_status (*personal_remove_utf8)(void* ctx, obi_utf8_view_v0 word);

    void (*destroy)(void* ctx);
} obi_spell_session_api_v0;

struct obi_spell_session_v0 {
    const obi_spell_session_api_v0* api;
    void* ctx;
};

typedef struct obi_text_spellcheck_v0 obi_text_spellcheck_v0;

typedef struct obi_text_spellcheck_api_v0 {
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;
    uint32_t reserved;
    uint64_t caps;

    /* Create a spellcheck session for a BCP47 language tag (UTF-8, NUL-terminated). */
    obi_status (*session_create)(void* ctx, const char* language_tag, obi_spell_session_v0* out_session);
} obi_text_spellcheck_api_v0;

struct obi_text_spellcheck_v0 {
    const obi_text_spellcheck_api_v0* api;
    void* ctx;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBI_PROFILE_TEXT_SPELLCHECK_V0_H */

