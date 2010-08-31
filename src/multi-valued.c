#include <curses.h>
#include <menu.h>

#include "ncurses.h"


int
main ()
{
    cpans_init_items();
    cpans_nc_init();
    cpans_nc_loop();
    cpans_nc_end();
    return 0;
}
