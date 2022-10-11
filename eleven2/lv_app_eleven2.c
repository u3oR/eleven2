/**
  * @file lv_app_eleven2.c
  */

#if 1

/*********************
*      INCLUDES
*********************/
#include "lv_app_eleven2.h"
#include "stdio.h"
#include "math.h"

/*********************
*  DEFINES / TYPEDEF
*********************/
const uint32_t __ELEVEN2_BLOCK_CELL_BG[12] = {
    ELEVEN2_COLOR_MAKE_HEX(205, 193, 180) ,  //1     1<<0
    ELEVEN2_COLOR_MAKE_HEX(238, 228, 218) ,  //2     1<<1
    ELEVEN2_COLOR_MAKE_HEX(238, 225, 201) ,  //4     1<<2
    ELEVEN2_COLOR_MAKE_HEX(243, 178, 122) ,  //8     1<<3
    ELEVEN2_COLOR_MAKE_HEX(246, 150, 100) ,  //16    1<<4
    ELEVEN2_COLOR_MAKE_HEX(247, 124, 95)  ,  //32    1<<5
    ELEVEN2_COLOR_MAKE_HEX(247, 100, 61)  ,  //64    1<<6
    ELEVEN2_COLOR_MAKE_HEX(237, 208, 115) ,  //128   1<<7
    ELEVEN2_COLOR_MAKE_HEX(237, 208, 115) ,  //256   1<<8
    ELEVEN2_COLOR_MAKE_HEX(237, 208, 115) ,  //512   1<<9
    ELEVEN2_COLOR_MAKE_HEX(237, 208, 115) ,  //1024  1<<10
    ELEVEN2_COLOR_MAKE_HEX(237, 208, 115) ,  //2048  1<<11
};

/**********************
*  STATIC VARIABLES
**********************/
#define ROW     4
#define COLUMN  4

static lv_obj_t* eleven2mgmt_curtScore  = nullptr;
static uint32_t  eleven2mgmt_data_score = 0;

/*********************
*  Block 
*********************/
typedef struct {
    lv_obj_t *block;
    lv_label_t* label;
    uint16_t val;
    uint32_t color;
} Block;

Block Blocks[ROW][COLUMN];

/**
 * @brief 清空数值,和颜色
 * @param B 
*/
void init(Block B[ROW][COLUMN]) {

    for (uint8_t i = 0; i < ROW; i++) {

        for (uint8_t j = 0; j < COLUMN; j++) {
            B[i][j].val    = 0;
            B[i][j].color  = ELEVEN2_BLOCK_COLOR(0);
        }
    }
}

void create(Block B[ROW][COLUMN]) {
    uint16_t cnt = 0;
    uint16_t pos[16] = {0};
    /*把空的位置都找出来放到pos中*/
    for (uint8_t i = 0; i < ROW; i++) {
        for (uint8_t j = 0; j < COLUMN; j++) {
            if (B[i][j].val == 0) {
                pos[cnt++] = i * 4 + j + 1;
            }
        }
    }
    /*多于两个空位置就在里面随机挑两个填入数值*/
    if (cnt >= 2) {
        uint16_t p1 = lv_rand(0, cnt);
        uint16_t p2 = lv_rand(0, cnt);
        Blocks[(pos[p1 - 1] / 4)][(pos[p1 - 1] % 4)].val = lv_rand(1, 2);
        Blocks[(pos[p2 - 1] / 4)][(pos[p2 - 1] % 4)].val = lv_rand(1, 2);
    }
    /*只剩下一个空位置就在这个空位置里面填入数值*/
    else if(cnt == 1) {
        Blocks[(pos[0] / 4)][(pos[0] % 4)].val = lv_rand(1, 2);
    }

}
/**
 * @brief 移动方块
 * @param B 
 * @param d 
*/
void move(Block B[ROW][COLUMN],unsigned char d) {
    switch (d)
    {
    case 0/*W*/:
        for (uint8_t i = 0; i < ROW - 1; i++) {
            for (uint8_t j = 0; j < COLUMN; j++) {
                if ((B[i][j].val == B[i + 1][j].val) && (B[i][j].val != 0)) {
                    B[i][j].val += 1;
                    B[i + 1][j].val = 0;
                }
            }
        }
        for (uint8_t j = 0; j < COLUMN; j++) {
            int k = 0;
            for (uint8_t i = 0; i < ROW; i++) {
                if (B[i][j].val != 0) {
                    B[k++][j].val = B[i][j].val;
                }
            }
            for (uint8_t i = k; i < ROW; i++) {
                B[i][j].val = 0;
            }
        }
        break;
    case 1/*A*/:
        for (uint8_t j = 0; j < COLUMN - 1; j++) {
            for (uint8_t i = 0; i < ROW; i++) {
                if ((B[i][j].val == B[i][j + 1].val) && (B[i][j].val != 0)) {
                    B[i][j].val += 1;
                    B[i][j+1].val = 0;
                }
            }
        }
        for (uint8_t i = 0; i < ROW; i++) {
            int k = 0;
            for (uint8_t j = 0; j < COLUMN; j++) {
                if (B[i][j].val != 0) {
                    B[i][k++].val = B[i][j].val;
                }
            }
            for (uint8_t j = k; j < COLUMN; j++) {
                B[i][j].val = 0;
            }
        }
        break;
    case 2/*S*/:
        for (uint8_t j = 0; j < COLUMN; j++)
        {
            for (int8_t i = ROW - 1; i >= 0; i--)
            {
                if ((B[i][j].val == B[i - 1][j].val) && (B[i][j].val != 0)) {
                    B[i][j].val += 1;
                    B[i - 1][j].val = 0;
                }
            }
        }
        for (uint8_t j = 0; j < COLUMN; j++)
        {
            int k = ROW - 1;
            for (int8_t i = ROW - 1; i >= 0; i--)
            {
                if (B[i][j].val != 0) {
                    B[k--][j].val = B[i][j].val;
                }
            }
            for (int8_t i = k; i >= 0; i--)
            {
                B[i][j].val = 0;
            }
        }
        break;
    case 3/*D*/:
        for (uint8_t i = 0; i < ROW; i++) {
            for (uint8_t j = COLUMN - 1; j >= 1; j--)
            {
                if (B[i][j].val == B[i][j-1].val && B[i][j].val != 0) {
                    B[i][j].val += 1;
                    B[i][j - 1].val = 0;
                }
            }
        }
        for (uint8_t i = 0; i < ROW; i++)
        {
            int k = ROW - 1;
            for (int8_t j = COLUMN - 1; j >= 0; j--)
            {
                if (B[i][j].val != 0) {
                    B[i][k--].val = B[i][j].val;
                }
            }
            for (int8_t j = k; j >= 0; j--)
            {
                B[i][j].val = 0;
            }
        }
        break;
    default/*None*/:
        break;
    }
}
/**
 * @brief 刷新方块颜色，刷新色块文本颜色
 * @param B 
*/
void update(Block B[ROW][COLUMN]) {

    for (uint8_t i = 0; i < ROW; i++) {
        for (uint8_t j = 0; j < COLUMN; j++) {
            /*读取数值对应的颜色*/
            B[i][j].color = ELEVEN2_BLOCK_COLOR(B[i][j].val);
            /*方块有值则赋值*/
            if (B[i][j].val != 0) {
                lv_label_set_text_fmt(B[i][j].label, "%ld", 1 << B[i][j].val);
            }
            /*方块无值则显示空格(不显示数值)*/
            else{
                lv_label_set_text_fmt(B[i][j].label, " ");
            }
            /*更新方块的样色*/
            lv_obj_set_style_bg_color(
                B[i][j].block,
                lv_color_hex(B[i][j].color),
                0
            );
            
        }
    }
}

/**********************
*   PRIVATE FUNCTIONS
**********************/

/*UI interface*/
static void eleven2_menu_create(lv_obj_t* parent);
static void eleven2_mainwindow_create(lv_obj_t *parent);
static void eleven2_ctrl_buttons_create(lv_obj_t* parent);
/*event callback function*/
static void eleven2_restart_event_callback(lv_event_t* e);
static void eleven2_ctrl_button_event_callback(lv_event_t* e);

/********************** 
*   GLOBAL FUNCTIONS
**********************/

/**
 * @brief Main function
 * @param None
*/
void lv_app_eleven2(void) {

    init(Blocks);

    //背景样式
    static lv_style_t bg_style;
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, lv_color_hex(ELEVEN2_THEME_BG));

    /*主页面框架*/
    lv_obj_t* main_frame = lv_obj_create(lv_scr_act());
    lv_obj_set_size(   main_frame, SCREEN_HOR, SCREEN_VER);/*全屏设置*/
    lv_obj_add_style(  main_frame, &bg_style, 0);
    lv_obj_clear_flag( main_frame, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_layout( main_frame, LV_LAYOUT_GRID);

    static lv_coord_t col_dsc[] = { LV_GRID_FR(9), LV_GRID_FR(5), LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    lv_obj_set_grid_dsc_array(main_frame, col_dsc, row_dsc);

    /*菜单面板*/
    lv_obj_t* main_panel_menu = lv_obj_create(main_frame);
    lv_obj_clear_flag(main_panel_menu, LV_OBJ_FLAG_SCROLLABLE);
    /*竖屏设置*/
    //lv_obj_set_size(main_panel_menu,SCREEN_HOR,(SCREEN_HOR * 9 / 16));
    lv_obj_set_width(main_panel_menu, SCREEN_HOR);
    /*游戏主框*/
    lv_obj_t* main_panel_game = lv_obj_create(main_frame);
    lv_obj_set_size(main_panel_game, SCREEN_HOR, SCREEN_HOR);

    lv_obj_t* main_panel_ctrl = lv_obj_create(main_frame);
    lv_obj_set_width(main_panel_ctrl, SCREEN_HOR);

    eleven2_obj_set_grid_cell(main_panel_menu, 0, 1, 1, 1, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);    /*菜单布局*/
    eleven2_obj_set_grid_cell(main_panel_game, 0, 0, 1, 2, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);    /*游戏框布局*/
    eleven2_obj_set_grid_cell(main_panel_ctrl, 1, 1, 1, 1, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);    /*控制框布局*/

    /*菜单面板*/
    eleven2_menu_create(main_panel_menu);
    eleven2_mainwindow_create(main_panel_game);
    eleven2_ctrl_buttons_create(main_panel_ctrl);
}
/**
 * @brief Menu
 * @param parent 
 */
static void eleven2_menu_create(lv_obj_t* parent) {

    //设置背景颜色
    lv_obj_set_style_bg_color(parent, lv_color_hex(ELEVEN2_THEME_BG), 0);
    //网格布局
    lv_obj_set_layout(parent, LV_LAYOUT_GRID);
    lv_obj_set_style_pad_gap(parent, 9, 0);/*方块间距*/
    static lv_coord_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; /*2 columns with 100 and 400 ps width*/
    static lv_coord_t row_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; /*3 100 px tall rows*/
    lv_obj_set_grid_dsc_array(parent, col_dsc, row_dsc);
    //lv_obj_set_grid_align(parent,LV_GRID_ALIGN_CENTER,LV_GRID_ALIGN_CENTER);


    static lv_style_t btn_style;
    static lv_style_t score_style;
    lv_style_init(&btn_style);
    lv_style_init(&score_style);
    // 按钮样式设置
    lv_style_set_bg_color(&btn_style, lv_color_hex(ELEVEN2_THEME_BTN_BG));
    lv_style_set_text_color(&btn_style, lv_color_hex(ELEVEN2_THEME_BTN_TXT));
    lv_style_set_text_align(&btn_style, LV_STYLE_ALIGN);
    // 得分板样式设置
    lv_style_set_bg_color( &score_style, lv_color_hex(ELEVEN2_THEME_SCORE_BG));
    lv_style_set_text_color(&score_style, lv_color_hex(ELEVEN2_THEME_SCORE_TXT));
    lv_style_set_text_align(&score_style, LV_STYLE_ALIGN);

    lv_obj_t* _txt;
    //SCORE板
    lv_obj_t* score = lv_obj_create(parent);
    lv_obj_set_size(score, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_clear_flag(score, LV_OBJ_FLAG_SCROLLABLE);
    eleven2mgmt_curtScore = lv_label_create(score);
    lv_label_set_text_fmt(eleven2mgmt_curtScore, "%d", eleven2mgmt_data_score);
    lv_obj_center(eleven2mgmt_curtScore);
    lv_obj_add_style(score, &score_style, 0);
    //<新游戏>按钮
    lv_obj_t* restart_btn = lv_btn_create(parent);
    lv_obj_set_size(restart_btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(restart_btn, &btn_style,0);
    _txt = lv_label_create(restart_btn);
    lv_label_set_text(_txt, "new");
    lv_obj_center(_txt);
    //按钮事件回调
    lv_obj_add_event_cb(restart_btn,eleven2_restart_event_callback,LV_EVENT_ALL,NULL);
    
    /*布局设置*/
    eleven2_obj_set_grid_cell(score,        0, 0, 1, 1, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);    /*SCORE布局*/
    eleven2_obj_set_grid_cell(restart_btn,  1, 0, 1, 1, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);    /*按钮布局 */
    
}

/**
 * @brief 主窗口
 * @param parent 
*/
static void eleven2_mainwindow_create(lv_obj_t* parent) {

    
    /*mainwindow背景样式*/
    lv_obj_set_style_bg_color(parent, lv_color_hex(ELEVEN2_THEME_BLOCK_BG), 0);/*背景颜色*/
    lv_obj_set_style_pad_gap(parent, 7, 0);/*方块间距*/
    lv_obj_set_layout(parent, LV_LAYOUT_GRID);/*方格布局*/

    /*为16个方块生成布局*/
    static lv_coord_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; 
    static lv_coord_t row_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; 
    lv_obj_set_grid_dsc_array(parent, col_dsc, row_dsc);

    /*设置生成16个方块*/
    for (uint8_t i = 0; i < ROW; i++)
    {
        for (uint8_t j = 0; j < COLUMN; j++)
        {
            /*只设置布局，不设置内容*/
            /*Block Container*/
            Blocks[i][j].block = lv_obj_create(parent);
            lv_obj_set_size(Blocks[i][j].block, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_clear_flag(Blocks[i][j].block, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(Blocks[i][j].block,lv_color_hex(ELEVEN2_BLOCK_COLOR(0)),0);//块背景
            Blocks[i][j].label = lv_label_create(Blocks[i][j].block);
            lv_label_set_text_fmt(Blocks[i][j].label, " ");
            lv_obj_center(Blocks[i][j].label);
            /*Grid Layout*/
            eleven2_obj_set_grid_cell(Blocks[i][j].block, i, j, 1, 1, LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);
        }
    }

    //lv_obj_t* target = eleven2mgmt_block[0][0];
    //lv_obj_update_layout(target);
    //lv_obj_t* t = lv_obj_create(lv_scr_act());
    //lv_obj_set_pos(t,
    //    target->coords.x1,
    //    target->coords.y1  );
    //lv_obj_set_size(t,
    //    target->coords.x2 - target->coords.x1 + 1,
    //    target->coords.y2 - target->coords.y1 + 1  );
    //lv_obj_set_style_bg_color(t, lv_color_hex(ELEVEN2_COLOR_MediumSeaGreen),0);

}

/**
 * @brief 方向控制按钮
 * @param parent 
*/
static void eleven2_ctrl_buttons_create(lv_obj_t* parent) {

    /*BackGround*/
    lv_obj_set_style_bg_color(
        parent,
        lv_color_hex(ELEVEN2_THEME_BLOCK_BG),
        0
    );
    lv_obj_set_style_pad_gap(parent, 3, 0);/*方块间距*/
    lv_obj_set_style_outline_width(parent, 0, 0);
    lv_obj_set_layout(parent, LV_LAYOUT_GRID);/*方格布局*/

    /*网格布局:两行三列*/
    static lv_coord_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    lv_obj_set_grid_dsc_array(parent, col_dsc, row_dsc);

    /*设置WASD按钮及样式*/
    static lv_obj_t* ctrl_button[4];
    lv_obj_t* _txt;
    for (uint8_t i = 0; i < 4; i++) {
        ctrl_button[i] = lv_btn_create(parent);
        lv_obj_set_style_bg_color(
            ctrl_button[i],
            lv_color_hex(ELEVEN2_COLOR_MediumSeaGreen),
            0
        );
        /* WASD Button layout */
        _txt = lv_label_create(ctrl_button[i]);
        switch (i) {
            case 0:
                lv_label_set_text(_txt, "W");
                eleven2_obj_set_grid_cell(
                    ctrl_button[i], 0, 1, 1, 1,
                    LV_GRID_ALIGN_STRETCH,LV_GRID_ALIGN_STRETCH);
                break;
            case 1:
                lv_label_set_text(_txt, "A");
                eleven2_obj_set_grid_cell(
                    ctrl_button[i], 1, 0, 1, 1,
                    LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);
                break;
            case 2:
                lv_label_set_text(_txt, "S");
                eleven2_obj_set_grid_cell(
                    ctrl_button[i], 1, 1, 1, 1,
                    LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);
                break;
            case 3:
                lv_label_set_text(_txt, "D");
                eleven2_obj_set_grid_cell(
                    ctrl_button[i], 1, 2, 1, 1,
                    LV_GRID_ALIGN_STRETCH, LV_GRID_ALIGN_STRETCH);
                break;
            default:
                break;
        }
        lv_obj_center(_txt);
        //按钮事件回调
        lv_obj_add_event_cb(ctrl_button[i],eleven2_ctrl_button_event_callback,LV_EVENT_ALL,i);
    }
}

/**
 * @brief 新游戏按钮
 * @param e 
*/
static void eleven2_restart_event_callback(lv_event_t* e) {

    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {

        eleven2mgmt_data_score = 0;
        lv_label_set_text_fmt(eleven2mgmt_curtScore, "%d", eleven2mgmt_data_score);
        init(Blocks);
        create(Blocks);
        update(Blocks);
        LV_LOG_USER("%d", lv_rand(1, 2) * 2);
    }
}

/**
 * @brief 
 * @param e 
*/
static void eleven2_ctrl_button_event_callback(lv_event_t* e) {

    lv_event_code_t code = lv_event_get_code(e);
    uint8_t _para = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED) {
        /* 0:W 1:A 2:S 3:D */
        switch (_para) {
            case 0:
                LV_LOG_USER("Clicked Up");
                move(Blocks, 0);
                break;
            case 1:
                LV_LOG_USER("Clicked Left");
                move(Blocks, 1);
                break;
            case 2:
                LV_LOG_USER("Clicked Down");
                move(Blocks, 2);
                break;
            case 3:
                LV_LOG_USER("Clicked Right");
                move(Blocks, 3);
                break;
        }
        /*移动完方块后创建新的方块并更新布局*/
        move(Blocks, _para);
        create(Blocks);
        update(Blocks);
        lv_label_set_text_fmt(eleven2mgmt_curtScore, "%d", eleven2mgmt_data_score);
    }
    
}


#endif
