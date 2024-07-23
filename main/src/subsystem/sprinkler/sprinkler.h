#ifndef sprinkler_H
#define sprinkler_H

#include <stdlib.h>
#include <time.h>

typedef struct _sprinkler_prog_s sprinkler_prog_t;
typedef struct _sprinkler_zone_s sprinkler_zone_t;

void sprinkler_prog_init(sprinkler_prog_t *p);

size_t sprinkler_prog_get_interval(sprinkler_prog_t *p);
void sprinkler_prog_set_interval(sprinkler_prog_t *p, size_t interval);

struct tm * sprinkler_prog_get_start(sprinkler_prog_t *p);
void sprinkler_prog_set_start(sprinkler_prog_t *p, struct tm *);
struct tm * sprinkler_prog_get_end(sprinkler_prog_t *p);
void sprinkler_prog_set_end(sprinkler_prog_t *p, struct tm *);

struct tm * sprinkler_prog_get_next(sprinkler_prog_t *p);

int sprinkler_prog_get_scale(sprinkler_prog_t *p);
void sprinkler_prog_get_scale(sprinkler_prog_t *p, int scale);

size_t sprinkler_prog_get_zone_count(sprinkler_prog_t *p);


char *sprinkler_prog_get_name(sprinkler_prog_t *p);
void sprinkler_prog_set_name(sprinkler_prog_t *p, char *name);


#endif //sprinkler_H
