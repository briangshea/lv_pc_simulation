/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "widgets.h"
#include "stdio.h"
#include "time.h"

/*********************
 *      DEFINES
 *********************/
#define LV_NAV_BAR_CLASS &lv_nav_bar_class

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_nav_bar_s {
    lv_obj_t obj;               // base object data

    lv_obj_t * tl;              // top left widget
    lv_obj_t * title;           // View Title
    lv_obj_t * tr;              // top right widget
} _lv_nav_bar_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_nav_bar_class = {
    .base_class = &lv_obj_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_nav_bar_t)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_nav_bar_create(lv_obj_t *parent, const char *title) {
    LV_LOG_INFO("lv_nav_bar begin");

    lv_obj_t * obj = lv_obj_class_create_obj(&lv_nav_bar_class,parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj(obj);

    lv_nav_bar_t * _nav = (lv_nav_bar_t *)obj;
    //lv_obj_set_flex_flow(_nav, LV_FLEX_FLOW_ROW);

    _nav->tl = lv_clock_create(obj);
    LV_ASSERT_MALLOC(_nav->tl);
    lv_clock_set_interval(_nav->tl, 100);
    lv_obj_align(_nav->tl, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_width(_nav->tl, LV_PCT(33));

    _nav->title = lv_label_create(_nav);
    LV_ASSERT_MALLOC(_nav->title);
    lv_obj_align(_nav->title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_width(_nav->title, LV_PCT(33));
    lv_obj_set_style_text_align(_nav->title, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(_nav->title, title);

    return obj;
}
