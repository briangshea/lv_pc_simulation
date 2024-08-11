/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "lv_spinbuttons.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_spinbuttons_class
#define MY_TYPE  lv_spinbuttons_t
#define TO_MY_TYPE(obj) (MY_TYPE *)obj

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_spinbuttons_s {
    lv_spinbox_t obj;               // base object data
    lv_obj_t *lbutton;
    lv_obj_t *rbutton;
} _lv_spinbuttons_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_spinbuttons_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_spinbuttons_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_spinbuttons_event_cb(const lv_obj_class_t * class_p, lv_event_t * e);
static void lv_spinbox_increment_event_cb(lv_event_t *e);
static void lv_spinbox_decrement_event_cb(lv_event_t *e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_spinbuttons_class = {
    .constructor_cb = &lv_spinbuttons_constructor_cb,
    .destructor_cb = &lv_spinbuttons_destructor_cb,
    .event_cb = &lv_spinbuttons_event_cb,

    .name = "spinbuttons",

    .base_class = &lv_spinbox_class,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(MY_TYPE),

    //.editable = ,
    //.group_def = ,
    .theme_inheritable = 1
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
static void lv_spinbuttons_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");
    lv_spinbuttons_t *_obj = (lv_spinbuttons_t *)obj;
    int h = lv_obj_get_height(obj);
    int w = lv_obj_get_width(obj);

    _obj->lbutton = lv_button_create(obj);
    lv_obj_set_size(_obj->lbutton, h, h);
    lv_obj_align_to(_obj->lbutton, obj, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_image_src(_obj->lbutton, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(_obj->lbutton, lv_spinbox_increment_event_cb, LV_EVENT_ALL, NULL);

    _obj->rbutton = lv_button_create(obj);
    lv_obj_set_size(_obj->rbutton, h, h);
    lv_obj_align_to(obj, _obj->rbutton, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_image_src(_obj->rbutton, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(_obj->rbutton, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);

    lv_textarea_set_align(obj, LV_TEXT_ALIGN_CENTER);
    lv_spinbox_step_prev(obj);
    lv_spinbox_step_prev(obj);
}

static void lv_spinbuttons_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");
}

static void lv_spinbuttons_event_cb(const lv_obj_class_t * class_p, lv_event_t * e) {
    LV_LOG_INFO("begin: event code %d", lv_event_get_code(e));
    lv_result_t res = LV_RESULT_OK;
    res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RESULT_OK)
        return;

    switch(lv_event_get_code(e)) {
        case LV_EVENT_DELETE:
            lv_spinbuttons_t *_obj = (lv_spinbuttons_t *)lv_event_get_current_target_obj(e);
            if(_obj->lbutton) lv_obj_delete(_obj->lbutton);
            if(_obj->rbutton) lv_obj_delete(_obj->rbutton);
            break;
    }
}

static void lv_spinbox_increment_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *this = lv_event_get_current_target_obj(e);

    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(lv_obj_get_parent(this));
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *this = lv_event_get_current_target_obj(e);

    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(lv_obj_get_parent(this));
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_spinbuttons_create(lv_obj_t *parent) {
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj(obj);
    return obj;
}
