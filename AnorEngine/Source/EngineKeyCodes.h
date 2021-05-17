#pragma once

//COPIED AND PASTED FROM "glfw3.h" file.

#define ANOR_KEY_UNKNOWN            -1

/* Printable keys */
#define ANOR_KEY_SPACE              32
#define ANOR_KEY_APOSTROPHE         39  /* ' */
#define ANOR_KEY_COMMA              44  /* , */
#define ANOR_KEY_MINUS              45  /* - */
#define ANOR_KEY_PERIOD             46  /* . */
#define ANOR_KEY_SLASH              47  /* / */
#define ANOR_KEY_0                  48
#define ANOR_KEY_1                  49
#define ANOR_KEY_2                  50
#define ANOR_KEY_3                  51
#define ANOR_KEY_4                  52
#define ANOR_KEY_5                  53
#define ANOR_KEY_6                  54
#define ANOR_KEY_7                  55
#define ANOR_KEY_8                  56
#define ANOR_KEY_9                  57
#define ANOR_KEY_SEMICOLON          59  /* ; */
#define ANOR_KEY_EQUAL              61  /* = */
#define ANOR_KEY_A                  65
#define ANOR_KEY_B                  66
#define ANOR_KEY_C                  67
#define ANOR_KEY_D                  68
#define ANOR_KEY_E                  69
#define ANOR_KEY_F                  70
#define ANOR_KEY_G                  71
#define ANOR_KEY_H                  72
#define ANOR_KEY_I                  73
#define ANOR_KEY_J                  74
#define ANOR_KEY_K                  75
#define ANOR_KEY_L                  76
#define ANOR_KEY_M                  77
#define ANOR_KEY_N                  78
#define ANOR_KEY_O                  79
#define ANOR_KEY_P                  80
#define ANOR_KEY_Q                  81
#define ANOR_KEY_R                  82
#define ANOR_KEY_S                  83
#define ANOR_KEY_T                  84
#define ANOR_KEY_U                  85
#define ANOR_KEY_V                  86
#define ANOR_KEY_W                  87
#define ANOR_KEY_X                  88
#define ANOR_KEY_Y                  89
#define ANOR_KEY_Z                  90
#define ANOR_KEY_LEFT_BRACKET       91  /* [ */
#define ANOR_KEY_BACKSLASH          92  /* \ */
#define ANOR_KEY_RIGHT_BRACKET      93  /* ] */
#define ANOR_KEY_GRAVE_ACCENT       96  /* ` */
#define ANOR_KEY_WORLD_1            161 /* non-US #1 */
#define ANOR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define ANOR_KEY_ESCAPE             256
#define ANOR_KEY_ENTER              257
#define ANOR_KEY_TAB                258
#define ANOR_KEY_BACKSPACE          259
#define ANOR_KEY_INSERT             260
#define ANOR_KEY_DELETE             261
#define ANOR_KEY_RIGHT              262
#define ANOR_KEY_LEFT               263
#define ANOR_KEY_DOWN               264
#define ANOR_KEY_UP                 265
#define ANOR_KEY_PAGE_UP            266
#define ANOR_KEY_PAGE_DOWN          267
#define ANOR_KEY_HOME               268
#define ANOR_KEY_END                269
#define ANOR_KEY_CAPS_LOCK          280
#define ANOR_KEY_SCROLL_LOCK        281
#define ANOR_KEY_NUM_LOCK           282
#define ANOR_KEY_PRINT_SCREEN       283
#define ANOR_KEY_PAUSE              284
#define ANOR_KEY_F1                 290
#define ANOR_KEY_F2                 291
#define ANOR_KEY_F3                 292
#define ANOR_KEY_F4                 293
#define ANOR_KEY_F5                 294
#define ANOR_KEY_F6                 295
#define ANOR_KEY_F7                 296
#define ANOR_KEY_F8                 297
#define ANOR_KEY_F9                 298
#define ANOR_KEY_F10                299
#define ANOR_KEY_F11                300
#define ANOR_KEY_F12                301
#define ANOR_KEY_F13                302
#define ANOR_KEY_F14                303
#define ANOR_KEY_F15                304
#define ANOR_KEY_F16                305
#define ANOR_KEY_F17                306
#define ANOR_KEY_F18                307
#define ANOR_KEY_F19                308
#define ANOR_KEY_F20                309
#define ANOR_KEY_F21                310
#define ANOR_KEY_F22                311
#define ANOR_KEY_F23                312
#define ANOR_KEY_F24                313
#define ANOR_KEY_F25                314
#define ANOR_KEY_KP_0               320
#define ANOR_KEY_KP_1               321
#define ANOR_KEY_KP_2               322
#define ANOR_KEY_KP_3               323
#define ANOR_KEY_KP_4               324
#define ANOR_KEY_KP_5               325
#define ANOR_KEY_KP_6               326
#define ANOR_KEY_KP_7               327
#define ANOR_KEY_KP_8               328
#define ANOR_KEY_KP_9               329
#define ANOR_KEY_KP_DECIMAL         330
#define ANOR_KEY_KP_DIVIDE          331
#define ANOR_KEY_KP_MULTIPLY        332
#define ANOR_KEY_KP_SUBTRACT        333
#define ANOR_KEY_KP_ADD             334
#define ANOR_KEY_KP_ENTER           335
#define ANOR_KEY_KP_EQUAL           336
#define ANOR_KEY_LEFT_SHIFT         340
#define ANOR_KEY_LEFT_CONTROL       341
#define ANOR_KEY_LEFT_ALT           342
#define ANOR_KEY_LEFT_SUPER         343
#define ANOR_KEY_RIGHT_SHIFT        344
#define ANOR_KEY_RIGHT_CONTROL      345
#define ANOR_KEY_RIGHT_ALT          346
#define ANOR_KEY_RIGHT_SUPER        347
#define ANOR_KEY_MENU               348

#define ANOR_KEY_LAST               ANOR_KEY_MENU