#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>
#include "cpans.h"




extern int search(const char * pattern);

int module_filter( regex_t * reg , moduledata_t * mitem );

// module list

moduledata_t ** modulelist_new( int size );

void modulelist_free( moduledata_t ** mlist , int size );

void modulelist_read( moduledata_t ** mlist , int num , FILE * in );

moduledata_t ** modulelist_filter( regex_t * reg , moduledata_t * mlist );

#endif
