#include "sprinkler.h"

#ifndef SPRINKLER_MAX_NAME_LEN
# define SPRINKLER_MAX_NAME_LEN 32
#endif

#ifndef SPRINKLER_MAX_ZONES
# define SPRINKLER_MAX_PROGS 8
#endif

#ifndef SPRINKLER_MAX_ZONES
# define SPRINKLER_MAX_ZONES 8
#endif

/**
 * Sprinkler Program Model
 */
struct _sprinkler_prog_s {
    size_t program_id;              // program id
    size_t interval;                // number of days between program runs

    struct tm start;                // absolute program start time
    struct tm end;                  // absolute program end time
    struct tm next;                 // absolute next program run date

    int scale;                      // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%

    size_t zone_count;              // number of zones controlled by program
    sprinkler_zone_t **zones;       // array of zones to be controlled by program, null terminated list

    char *name;                     // name of the program
};

/**
 * Sprinkler Zone Model
 */
struct _sprinkler_zone_s {
    size_t zone_id;                 // zone number, 0 to MAX_ZONES-1

    size_t seconds;                 // number of seconds zone will run for.
    size_t scale;                   // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%

    int gpio_num;                   // gpio_num to enable for zone control.

    struct tm last_run;             // last run time
    struct tm next_run;             // next run time

    char *image;                    // zone image to display
    char *name;                     // zone name.
};
