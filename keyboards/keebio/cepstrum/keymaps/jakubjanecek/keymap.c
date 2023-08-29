// Copyright 2023 Danny Nguyen (danny@keeb.io)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    M_SCRS = SAFE_RANGE, // macOS screenshot
    M_BLD, // IDEA build
    M_FMT, // IDEA organize imports&format
    M_REN, // IDEA rename
    M_FND, // IDEA find usages
    M_RAY, // Raycast
    M_EMO, // Emoji picker
};

// tap dance
enum {
    TD_1_F1,
    TD_2_F2,
    TD_3_F3,
    TD_4_F4,
    TD_5_F5,
    TD_6_F6,
    TD_7_F7,
    TD_8_F8,
    TD_9_F9,
    TD_0_F10,
    TD_MINS_F11,
    TD_EQL_F12,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case M_SCRS:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_LCTL(SS_LGUI("4"))));
            } else {
                // when released
            }
            break;
        case M_BLD:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_F9)));
            }
            break;
        case M_FMT:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_O))));
                SEND_STRING(SS_LALT(SS_LGUI(SS_TAP(X_L))));
            }
            break;
        case M_REN:
            if (record->event.pressed) {
                SEND_STRING(SS_LSFT(SS_TAP(X_F6)));
            }
            break;
        case M_FND:
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_F7)));
            }
            break;
        case M_RAY:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI(SS_TAP(X_SPACE)));
            }
            break;
        case M_EMO:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_SPACE))));
            }
            break;
        case TD(TD_1_F1):
        case TD(TD_2_F2):
        case TD(TD_3_F3):
        case TD(TD_4_F4):
        case TD(TD_5_F5):
        case TD(TD_6_F6):
        case TD(TD_7_F7):
        case TD(TD_8_F8):
        case TD(TD_9_F9):
        case TD(TD_0_F10):
        case TD(TD_MINS_F11):
        case TD(TD_EQL_F12):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    [TD_1_F1] = ACTION_TAP_DANCE_TAP_HOLD(KC_1, KC_F1),
    [TD_2_F2] = ACTION_TAP_DANCE_TAP_HOLD(KC_2, KC_F2),
    [TD_3_F3] = ACTION_TAP_DANCE_TAP_HOLD(KC_3, KC_F3),
    [TD_4_F4] = ACTION_TAP_DANCE_TAP_HOLD(KC_4, KC_F4),
    [TD_5_F5] = ACTION_TAP_DANCE_TAP_HOLD(KC_5, KC_F5),
    [TD_6_F6] = ACTION_TAP_DANCE_TAP_HOLD(KC_6, KC_F6),
    [TD_7_F7] = ACTION_TAP_DANCE_TAP_HOLD(KC_7, KC_F7),
    [TD_8_F8] = ACTION_TAP_DANCE_TAP_HOLD(KC_8, KC_F8),
    [TD_9_F9] = ACTION_TAP_DANCE_TAP_HOLD(KC_9, KC_F9),
    [TD_0_F10] = ACTION_TAP_DANCE_TAP_HOLD(KC_0, KC_F10),
    [TD_MINS_F11] = ACTION_TAP_DANCE_TAP_HOLD(KC_MINS, KC_F11),
    [TD_EQL_F12] = ACTION_TAP_DANCE_TAP_HOLD(KC_EQL, KC_F12),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_65xt(
    M_FMT,        M_BLD,  QK_GESC, TD(TD_1_F1), TD(TD_2_F2), TD(TD_3_F3), TD(TD_4_F4), TD(TD_5_F5), TD(TD_6_F6), TD(TD_7_F7),    TD(TD_8_F8), TD(TD_9_F9), TD(TD_0_F10), TD(TD_MINS_F11), TD(TD_EQL_F12), KC_BSPC, KC_DEL,
    M_FND,        M_REN,  KC_TAB,  KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,           KC_I,        KC_O,        KC_P,         KC_LBRC,         KC_RBRC,        KC_BSLS, KC_HOME,
    KC_F5,        KC_F6,  KC_CAPS, KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,           KC_K,        KC_L,        KC_SCLN,      KC_QUOT,                         KC_ENT,  KC_PGUP,
    KC_F7,        KC_F8,  KC_LSFT, KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,           KC_COMM,     KC_DOT,      KC_SLSH,      KC_RSFT,         KC_UP,                   KC_PGDN,
    LAG(KC_SLSH), KC_F10, MO(1),   KC_LCTL,     KC_LALT,     KC_LGUI,     KC_SPC,                                MO(1),          M_RAY,       KC_RALT,     MO(1),        KC_LEFT,         KC_DOWN,        KC_RGHT
  ),
  [1] = LAYOUT_65xt(
    KC_KB_VOLUME_DOWN, KC_KB_VOLUME_UP, KC_GRV,        KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,   KC_F6,        KC_F7,        KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  QK_BOOT,     HYPR(KC_DEL),
    KC_KB_MUTE,        KC_MPLY,         _______,       _______,    HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), _______, EE_CLR,       _______,      KC_UP  , _______, _______, _______, _______, _______,     HYPR(KC_HOME),
    _______,           _______,         LALT(KC_BSPC), HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), _______, _______,      KC_LEFT,      KC_DOWN, KC_RGHT, _______, _______,          LSG(KC_ENT), M_SCRS,
    _______,           _______,         _______,       _______,    _______,    _______,    _______,    _______, LAG(KC_LEFT), LAG(KC_RGHT), _______, _______, _______, _______, _______,              _______,
    _______,           _______,         _______,       _______,    _______,    _______,    M_RAY,                                           _______, M_EMO,   _______, _______, _______, _______,     _______
  )
};
