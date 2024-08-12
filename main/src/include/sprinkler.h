#ifndef sprinkler_H
#define sprinkler_H

#include <stdlib.h>
#include <time.h>
#include "linkedlist.h"

#define SPRINKLER_SCALE_MIN 0           //    0%
#define SPRINKLER_SCALE_MAX 3000        //  300%
#define SPRINKLER_SCALE_M   1000        // 1000 = 100.0%

#define SPRINKLER_SCALE_FROM(x)  ((x)/SPRINKLER_SCALE_M)
#define SPRINKLER_SCALE_TO(x)    ((x)*SPRINKLER_SCALE_M)

typedef struct _sprinkler_prog_s sprinkler_prog_t;
typedef struct _sprinkler_zone_s sprinkler_zone_t;

typedef enum {
    SPRINKLER_EVENT_CHANGED = 0,
    SPRINKLER_EVENT_STARTED,
    SPRINKLER_EVENT_ENDED,
    SPRINKLER_EVENT_DELETED
} sprinkler_event_type_t;

typedef struct {
    unsigned char hour;
    unsigned char min;
} sprinkler_time_t;

typedef struct _sprinkler_prog_event_s sprinkler_prog_event_t;
typedef struct _sprinkler_zone_event_s sprinkler_zone_event_t;
typedef llnode_t* sprinkler_prog_dsc_t;

typedef void (*sprinkler_prog_event_cb)(sprinkler_prog_event_t *e);
typedef void (*sprinkler_zone_event_cb)(sprinkler_zone_event_t *e);

sprinkler_prog_t * sprinkler_prog_init(void);
void sprinkler_prog_delete(sprinkler_prog_t *p);

sprinkler_prog_dsc_t sprinkler_prog_add_cb(sprinkler_prog_t *p, sprinkler_prog_event_cb program_cb, void *user_data);
void sprinkler_prog_remove_cb(sprinkler_prog_t *p, sprinkler_prog_dsc_t dsc);

size_t sprinkler_prog_get_interval(sprinkler_prog_t *p);
void sprinkler_prog_set_interval(sprinkler_prog_t *p, size_t interval);

sprinkler_time_t const * sprinkler_prog_get_start(sprinkler_prog_t *p);
void sprinkler_prog_set_start(sprinkler_prog_t *p, sprinkler_time_t const * start);

time_t* sprinkler_prog_get_next(sprinkler_prog_t *p);
int sprinkler_prog_get_scale(sprinkler_prog_t *p);
void sprinkler_prog_set_scale(sprinkler_prog_t *p, int scale);
size_t sprinkler_prog_get_zone_count(sprinkler_prog_t *p);
const sprinkler_zone_t *sprinkler_prog_zone_at(sprinkler_prog_t *p, size_t at);
char *sprinkler_prog_get_name(sprinkler_prog_t *p);
void sprinkler_prog_set_name(sprinkler_prog_t *p, char const * name);
char *sprinkler_prog_get_description(sprinkler_prog_t *p);
void sprinkler_prog_set_description(sprinkler_prog_t *p, char const * description);
void sprinkler_prog_set_enabled(sprinkler_prog_t *p, int enabled);
int sprinkler_prog_get_enabled(sprinkler_prog_t *p);

sprinkler_zone_t * sprinkler_zone_init(sprinkler_zone_event_cb zone_cb, void *user_data);
void sprinkler_zone_delete(sprinkler_zone_t *);
time_t sprinkler_zone_get_duration(sprinkler_zone_t *z);
void sprinkler_zone_set_duration(sprinkler_zone_t *z, time_t duration);
int sprinkler_zone_get_scale(sprinkler_zone_t *z);
void sprinkler_zone_set_scale(sprinkler_zone_t *z, int scale);
time_t sprinkler_zone_last_run(sprinkler_zone_t *z);
time_t sprinkler_zone_next_run(sprinkler_zone_t *z);
const char * sprinkler_zone_get_image(sprinkler_zone_t *z);
void sprinkler_zone_set_image(sprinkler_zone_t *z, char * image);
const char * sprinkler_zone_get_name(sprinkler_zone_t *z);
void sprinkler_zone_set_name(sprinkler_zone_t *z, char * name);
const char * sprinkler_zone_get_description(sprinkler_zone_t *z);
void sprinkler_sone_set_description(sprinkler_zone_t *z, char *description);

sprinkler_event_type_t sprinkler_prog_event_get_type(sprinkler_prog_event_t *e);
sprinkler_prog_t * sprinkler_prog_event_get_prog(sprinkler_prog_event_t *e);
void * sprinkler_prog_event_get_user_data(sprinkler_prog_event_t *e);

sprinkler_event_type_t sprinkler_zone_event_get_type(sprinkler_zone_event_t *e);
sprinkler_zone_t * sprinkler_zone_event_get_zone(sprinkler_zone_event_t *e);
void * sprinkler_zone_event_get_user_data(sprinkler_zone_event_t *e);

#endif //sprinkler_H
