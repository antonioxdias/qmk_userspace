/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NUMNAV,
    LAYER_SYMBOLS,
    LAYER_FNMEDIA,
    LAYER_POINTER,
};

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define BSP_PTR LT(LAYER_POINTER, KC_BSPC)
#define ENT_FUN LT(LAYER_FNMEDIA, KC_ENT)
#define SPC_SYM LT(LAYER_SYMBOLS, KC_SPC)
#define TAB_NAV LT(LAYER_NUMNAV, KC_TAB)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE


enum custom_keycodes {
    KEYMAP = SAFE_RANGE,
    LOG,
    ARROWFUNC,
    LITERAL,
};

enum combos {
    JK_M1,
    KL_M3,
    COMBO_LENGTH,
};

uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM jk_combo[] = {LALT_T(KC_J), RGUI_T(KC_K), COMBO_END};
const uint16_t PROGMEM kl_combo[] = {RGUI_T(KC_K), RSFT_T(KC_L), COMBO_END};

combo_t key_combos[] = {
    [JK_M1] = COMBO(jk_combo, KC_BTN1),
    [KL_M3] = COMBO(kl_combo, KC_BTN3),
};

// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, \
       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, \
       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_MINS, \
                               BSP_PTR, ENT_FUN, SPC_SYM, TAB_NAV, KC_MPLY

#define LAYOUT_LAYER_NUMNAV                                                                   \
    XXXXXXX,    KC_7,    KC_8,    KC_9, XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, XXXXXXX, \
    XXXXXXX,    KC_4,    KC_5,    KC_6,    KC_0, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, \
    XXXXXXX,    KC_1,    KC_2,    KC_3, XXXXXXX, XXXXXXX, XXXXXXX, KC_COMM,  KC_DOT, XXXXXXX, \
                               KC_BSPC,  KC_ENT,  KC_SPC, KC_TRNS, XXXXXXX

#define LAYOUT_LAYER_SYMBOLS                                                                  \
    KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_AMPR, KC_SLSH, KC_QUOT, KC_DQUO, KC_ASTR, \
    KC_PIPE, LITERAL,ARROWFUNC,KC_LPRN, KC_LCBR, KC_RCBR, KC_RPRN, KC_QUES, KC_PEQL, KC_PLUS, \
    KC_BSLS, XXXXXXX, XXXXXXX,   KC_LT, KC_LBRC, KC_RBRC,   KC_GT, KC_TILD,  KC_GRV, KC_CIRC, \
                               KC_BSPC,  KC_ENT, KC_TRNS,  KC_TAB, XXXXXXX

#define LAYOUT_LAYER_FNMEDIA                                                                  \
    QK_BOOT,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, \
    KC_CAPS,   KC_F4,   KC_F5,   KC_F6,  KC_F11, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     LOG, \
    KC_PSCR,   KC_F1,   KC_F2,   KC_F3,  KC_F12, XXXXXXX, KC_BRMD, KC_BRMU, XXXXXXX,  KEYMAP, \
                                KC_DEL, KC_TRNS,  KC_SPC,  KC_ESC, KC_MPLY

#define LAYOUT_LAYER_POINTER                                                                  \
    DPI_MOD, XXXXXXX, SNIPING, DRGSCRL, XXXXXXX, XXXXXXX, DRGSCRL, SNIPING, XXXXXXX, DPI_MOD, \
    S_D_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, S_D_MOD, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN3, KC_BTN4, KC_BTN5, XXXXXXX, XXXXXXX, \
                               KC_TRNS, KC_BTN3, KC_BTN1, KC_BTN2, XXXXXXX

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LCTL_T(L10), LSFT_T(L11), LGUI_T(L12), LALT_T(L13),        L14,  \
             R15,  LALT_T(R16), RGUI_T(R17), RSFT_T(R18), RCTL_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00, _L_PTR(L01),         L02,         L03,         L04,  \
             R05,         R06,         R07, _L_PTR(R08),         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
             L20,         L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,         R29, \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_NUMNAV] = LAYOUT_wrapper(LAYOUT_LAYER_NUMNAV),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
  [LAYER_FNMEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_FNMEDIA),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case KEYMAP:
        if (record->event.pressed) {
          SEND_STRING("https://github.com/antonioxdias/qmk_userspace/blob/main/keyboards/bastardkb/charybdis/3x5/keymaps/antonioxdias/keymap.c");
        } else {
          // when keycode is released
        }
        break;
      case LOG:
        if (record->event.pressed) {
          SEND_STRING("console.log()"SS_TAP(X_LEFT));
        } else {
          // when keycode is released
        }
        break;
      case ARROWFUNC:
        if (record->event.pressed) {
          SEND_STRING("=>");
        } else {
          // when keycode is released
        }
        break;
      case LITERAL:
        if (record->event.pressed) {
          SEND_STRING("${}"SS_TAP(X_LEFT));
        } else {
          // when keycode is released
        }
        break;
  
      }
  
    return true;
}

static int8_t tab_ticks = 0;
static int8_t zoom_ticks = 0;

bool encoder_update_user(uint8_t index, bool ccw) {
    switch(biton32(layer_state)){
      case LAYER_POINTER:
        if (ccw) {
            tap_code(KC_WH_D);
        } else {
            tap_code(KC_WH_U);
        }
        break;
    case LAYER_FNMEDIA:
        if (ccw) { --tab_ticks; } else { ++tab_ticks; }
        if (abs(tab_ticks) < 2) { break; }

        if (ccw) {
            register_code16(KC_LCTL);
            register_code16(KC_LSFT);
            tap_code(KC_TAB);
            unregister_code16(KC_LSFT);
            unregister_code16(KC_LCTL);
        } else {
            register_code16(KC_LCTL);
            tap_code(KC_TAB);
            unregister_code16(KC_LCTL);
        }
        tab_ticks = 0;
        break;
    case LAYER_SYMBOLS:
        if (ccw) { --zoom_ticks; } else { ++zoom_ticks; }
        if (abs(zoom_ticks) < 2) { break; }

        if (ccw) {
            register_code16(KC_LALT);
            tap_code(KC_MINS);
            unregister_code16(KC_LALT);
        } else {
            register_code16(KC_LALT);
            register_code16(KC_PLUS);
            unregister_code16(KC_PLUS);
            unregister_code16(KC_LALT);
        }
        zoom_ticks = 0;
        break;
    case LAYER_NUMNAV:
        if (ccw) {
            tap_code(KC_LEFT);
        } else {
            tap_code(KC_RIGHT);
        }
        break;
    default:
        if (ccw) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    }
    return false;
}

// qmk compile -c -kb bastardkb/charybdis/3x5 -km antonioxdias