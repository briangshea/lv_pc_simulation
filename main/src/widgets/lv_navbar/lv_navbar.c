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
#define MY_CLASS    &lv_navbar_class
#define MY_TYPE     lv_navbar_t
#define TO_MY_OBJ(obj) MY_TYPE * my_##obj = (MY_TYPE *)obj

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_navbar_s {
    lv_obj_t obj;               // base object data

    lv_obj_t * tl;              // top left widget
    lv_obj_t * title;           // View Title
    lv_obj_t * tr;              // top right widget
} _lv_navbar_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_navbar_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_navbar_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_navbar_event_cb(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_navbar_class = {
    .constructor_cb = &lv_navbar_constructor_cb,
    .destructor_cb = &lv_navbar_destructor_cb,
    //.event_cb = &lv_panel_event_cb,

    .name = "navbar",
    .base_class = &lv_obj_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_navbar_t),

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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_navbar_create(lv_obj_t *parent) {
    LV_LOG_INFO("lv_navbar begin");

    lv_obj_t * obj = lv_obj_class_create_obj(&lv_navbar_class,parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj(obj);
    TO_MY_OBJ(obj);

    return my_obj;
}

static void lv_navbar_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_ASSERT_OBJ(obj, MY_CLASS);
    TO_MY_OBJ(obj);

    my_obj->tl = lv_clock_create(my_obj);
    LV_ASSERT_MALLOC(my_obj->tl);
    lv_clock_set_interval(my_obj->tl, 100);
    lv_obj_align(my_obj->tl, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_width(my_obj->tl, LV_PCT(33));

    my_obj->title = lv_label_create(my_obj);
    LV_ASSERT_MALLOC(my_obj->title);
    lv_obj_align(my_obj->title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_width(my_obj->title, LV_PCT(33));
    lv_obj_set_style_text_align(my_obj->title, LV_TEXT_ALIGN_CENTER, 0);
}

static void lv_navbar_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_ASSERT_OBJ(obj, MY_CLASS);
    TO_MY_OBJ(obj);

    if(my_obj->tr)
        lv_obj_delete(my_obj->tr);

    if(my_obj->tl)
        lv_obj_delete(my_obj->tl);

    if(my_obj->title)
        lv_obj_delete(my_obj->title);

}

static void lv_navbar_event_cb(const lv_obj_class_t * class_p, lv_event_t * e) {

}

void lv_navbar_set_title(lv_obj_t *obj, const char *title) {
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT(title);
    TO_MY_OBJ(obj);

    lv_label_set_text(my_obj->title, title);
}
