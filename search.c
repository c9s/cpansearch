#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

#include <stdarg.h>
#include <sys/types.h>
#include <regex.h>
#include <glib.h>
#include <ctype.h>

#include "membuf.h"
#include "cpans.h"

extern char version[];
extern char ignore_case;
extern char fullurl;

void cpansearch_datafile(char * path )
{
    sprintf( path , "%s/.cpansearch.dat" , g_get_home_dir() );
}


int search(const char * pattern)
{

    char datafile[128];
    regex_t reg;
    FILE * in;
    moduledata_t mdata;
    sourcemeta_t smeta;
    char * url;

    cpansearch_datafile( datafile );

    in = fopen (datafile, "rb+");
    assert( in != NULL );

    fread( &smeta , sizeof(sourcemeta_t) , 1 , in );
    url = smeta.uri;
    printf( "Source list from: %s\n" , url );


    char hosturl[128];
    char * c = strstr( url , "/modules" );
    strncpy( hosturl , url , c - url ); 


    int flag = REG_NOSUB | REG_EXTENDED;
    if( ignore_case )
        flag = flag | REG_ICASE;

    assert( regcomp( &reg , pattern , flag ) == 0 );

    regmatch_t matchlist[1];

    char longurl[300];
    while( !feof(in) ) {
        memset( &mdata , 0 , sizeof(moduledata_t) );
        fread( &mdata , sizeof(moduledata_t) , 1 , in );

        if( regexec( &reg , mdata.name  , 1 , matchlist , 0 ) == 0 ) {
            if( fullurl ) {
                sprintf( longurl , "%s/authors/id/%s" , hosturl , mdata.path );
                printf( "%-40s - %s (%s)\n" , mdata.name , mdata.version , longurl );
            }
            else {
                printf( "%-40s - %s (%s)\n" , mdata.name , mdata.version , mdata.path );
            }
        }
    }
    fclose(in);
    return 0;
}


