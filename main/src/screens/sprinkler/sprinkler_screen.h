#ifndef screen_sprinkler_H
#define screen_sprinkler_H

#include "lvgl/lvgl.h"
#include "sprinkler.h"

#ifdef __cplusplus
    extern "C" {
#endif

void screen_sprinkler_display(sprinkler_prog_t *activeProg);

#ifdef __cplusplus
    }
#endif

#endif //screen_sprinkler_H
