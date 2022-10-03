#if 1

#ifndef _LV_APP_ELEVEN_2_H_
#define _LV_APP_ELEVEN_2_H_

#ifdef __cplusplus
extern "C" {
#endif
/*********************
*      INCLUDES
*********************/
#include "../../lv_app.h"
/*********************
*      DEFINES
*********************/

// #define ELEVEN2_COLOR_MAKE(RGB) (lv_color_t)LV_COLOR_MAKE( (RGB>>16) & 0xFF,(RGB>>8) & 0xFF,(RGB) & 0xFF)
#define ELEVEN2_COLOR_MAKE_HEX(R,G,B) (uint32_t)((R << 16) + (G << 8) + B)

/*--------------
* App Config
---------------*/
//屏幕分辨率
#define SCREEN_HOR 480
#define SCREEN_VER 272

#define ELEVEN_SCREEN_HOR SCREEN_HOR
#define ELEVEN_SCREEN_VER SCREEN_VER
//块数量 4 * 4
#define ELEVEN2_BLOCK_ROW_NUM 4
#define ELEVEN2_BLOCK_COL_NUM 4
/*--------------
* Color Define
* 颜色命名 https://www.matools.com/color
* 调色板   https://www.sojson.com/web/panel.html
---------------*/
#define ELEVEN2_COLOR_Peru              0xCD853F 
#define ELEVEN2_COLOR_Coral             0xFF7F50
#define ELEVEN2_COLOR_LightGrey         0xD5DBDB
#define ELEVEN2_COLOR_White             0xFFFFFF
#define ELEVEN2_COLOR_FloralWhite       0xFFFAF0
#define ELEVEN2_COLOR_MediumSeaGreen    0x3CB371
#define ELEVEN2_COLOR_Tan               0xD2B48C
#define ELEVEN2_COLOR_Bisque            0xFFE4C4
/*--------------
* Theme Config 
---------------*/
//得分板主题
#define ELEVEN2_THEME_SCORE_BG          ELEVEN2_COLOR_MAKE_HEX(187, 173, 160)
#define ELEVEN2_THEME_SCORE_TXT         ELEVEN2_COLOR_White
//按钮主题
#define ELEVEN2_THEME_BTN_BG            ELEVEN2_COLOR_MAKE_HEX(143, 122, 102)
#define ELEVEN2_THEME_BTN_TXT           ELEVEN2_COLOR_MAKE_HEX(249, 246, 242)
//背景主题
#define ELEVEN2_THEME_BG                ELEVEN2_COLOR_MAKE_HEX(250, 248, 239)
//块主题
#define ELEVEN2_THEME_BLOCK_BG          ELEVEN2_COLOR_MAKE_HEX(187, 173, 160)
//块中数字颜色
#define ELEVEN2_THEME_BLOCK_TXT_DARK    ELEVEN2_COLOR_MAKE_HEX(119, 110, 101)
#define ELEVEN2_THEME_BLOCK_TXT_LIGHT   ELEVEN2_COLOR_MAKE_HEX(249, 246, 242)
//块颜色 1代表背景 其余代表数字所指的颜色
extern const uint32_t __ELEVEN2_BLOCK_CELL_BG[12];
#define ELEVEN2_BLOCK_COLOR(x) __ELEVEN2_BLOCK_CELL_BG[x]

/**********************
* GLOBAL PROTOTYPES
**********************/
void lv_app_eleven2(void);
/**********************
*      MACROS
**********************/
#define nullptr ((void *)0)

/*Just adjust the order of parameters*/
#define eleven2_obj_set_grid_cell(obj,x,y,size_x,size_y,align_x,align_y) \
        lv_obj_set_grid_cell(obj,align_y, y, size_x,align_x, x, size_y)


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif


#endif
