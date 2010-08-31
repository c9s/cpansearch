#ifndef __NCURSES_H__
#define __NCURSES_H__

#include "cpans.h"

void cpans_nc_init( moduledata_t ** mlist , size_t mlistsize );

void cpans_nc_loop();

void cpans_nc_end(void);

#endif
