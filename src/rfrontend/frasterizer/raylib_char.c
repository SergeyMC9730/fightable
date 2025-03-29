
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>

#ifdef TARGET_SUPPORTS_X11
#define XK_LATIN1
#define XK_3270
#include <X11/keysym.h>

int __char_table[] = {
    KEY_NULL,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    XK_apostrophe,
    XK_comma,
    XK_minus,
    XK_period,
    XK_slash,
    XK_0,
    XK_1,
    XK_2,
    XK_3,
    XK_4,
    XK_5,
    XK_6,
    XK_7,
    XK_8,
    XK_9,
    XK_semicolon,
    XK_equal,
    XK_A,
    XK_B,
    XK_C,
    XK_D,
    XK_E,
    XK_F,
    XK_G,
    XK_H,
    XK_I,
    XK_J,
    XK_K,
    XK_L,
    XK_M,
    XK_N,
    XK_O,
    XK_P,
    XK_Q,
    XK_R,
    XK_S,
    XK_T,
    XK_U,
    XK_V,
    XK_W,
    XK_X,
    XK_Y,
    XK_Z,
    XK_bracketleft,
    XK_backslash,
    XK_bracketright,
    XK_grave,
    XK_space,
    XK_Escape,
    XK_ISO_Enter,
    XK_Tab,
    XK_BackSpace,
    XK_Insert,
    XK_Delete,
    XK_Right,
    XK_Left,
    XK_Down,
    XK_Up,
    XK_Page_Up,
    XK_Page_Down,
    XK_Home,
    XK_End,
    XK_Caps_Lock,
    XK_Scroll_Lock,
    XK_Num_Lock,
    XK_3270_PrintScreen,
    XK_Pause,
    XK_F1,
    XK_F2,
    XK_F3,
    XK_F4,
    XK_F5,
    XK_F6,
    XK_F7,
    XK_F8,
    XK_F9,
    XK_F10,
    XK_F11,
    XK_F12,
    XK_Shift_L,
    XK_Control_L,
    XK_Alt_L,
    XK_Super_L,
    XK_Shift_R,
    XK_Control_R,
    XK_Alt_R,
    XK_Super_R,
    XK_Menu,
    XK_KP_0,
    XK_KP_1,
    XK_KP_2,
    XK_KP_3,
    XK_KP_4,
    XK_KP_5,
    XK_KP_6,
    XK_KP_7,
    XK_KP_8,
    XK_KP_9,
    XK_KP_Decimal,
    XK_KP_Divide,
    XK_KP_Multiply,
    XK_KP_Subtract,
    XK_KP_Add,
    XK_KP_Enter,
    XK_KP_Equal,
    KEY_NULL, /* android key KEY_BACK, not implemented in X11 */
    KEY_NULL, /* android key KEY_MENU, not implemented in X11 */
    KEY_NULL, /* android key KEY_VOLUME_UP, not implemented in X11 */
    KEY_NULL  /* android key KEY_VOLUME_DOWN, not implemented in X11 */
};
int __char_table_size = sizeof(__char_table) / sizeof(int);
#endif

int _fRasterizerRaylibCharToNative(int rc) {
#ifdef TARGET_SUPPORTS_X11
    rc = __char_table[rc % __char_table_size];
#endif
    return rc;
}

int _fRasterizerNativeCharToIndex(int nc) {
#ifdef TARGET_SUPPORTS_X11
    for (int i = 0; i < __char_table_size; i++) {
        if (__char_table[i] == nc) return i;
    }
#endif
    return 0;
}
