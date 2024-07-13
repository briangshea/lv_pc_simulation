/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "widgets.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_panel_class
#define MY_TYPE  lv_panel_t
#define TO_MY_TYPE(obj) (MY_TYPE *)obj

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_panel_s {
    lv_obj_t obj;               // base object data

    lv_obj_t * title;           // View Title
} _lv_panel_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_panel_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_panel_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_panel_event_cb(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_panel_class = {
    //.constructor_cb = &lv_panel_constructor_cb,
    //.destructor_cb = &lv_panel_destructor_cb,
    //.event_cb = &lv_panel_event_cb,

    .name = "panel",

    .base_class = &lv_obj_class,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_panel_t),

    //.editable = ;
    //.group_def = ;
    //.theme_inheritable = ;
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
static void lv_panel_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");
}

static void lv_panel_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");

}

static void lv_panel_event_cb(const lv_obj_class_t * class_p, lv_event_t * e) {
    LV_LOG_INFO("begin");

}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_panel_create(lv_obj_t *parent) {
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS,parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj(obj);
    return obj;
}
