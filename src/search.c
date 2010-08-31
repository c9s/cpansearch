
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

#include "cpans.h"
#include "utils.h"

extern char version[];
extern char ignore_case;
extern char verbose;
extern char ncurses;

void
render_search_ncurses (
    moduledata_t ** mlist,
    int mlistsize,
    regex_t * reg)
{




}

moduledata_t ** modulelist_filter( regex_t * reg , moduledata_t * mlist )
{


}

int module_filter( regex_t * reg , moduledata_t * mitem )
{
    regmatch_t reglist[1];
    if( regexec( reg , mitem->name  , 1 , reglist , 0 ) != 0 )
        return 0;

    // XXX: add an opt for ignoring 0 version modules
    if( strcmp(mitem->version,"0") == 0 )
        return 0;

    return 1;
}


/* level 1 search result rendering
   very simple , just render module name list
 * */
void
render_search(
    moduledata_t ** mlist,
    int mlistsize,
    regex_t * reg
    )
{
    int i;
    moduledata_t * mitem;
    for(i=0;i<mlistsize;i++) {
        if( module_filter( reg , mlist[i] ) == 0 )
            continue;
        mitem = mlist[i];
        printf( "%s\n" , mitem->name );
    }
}


void
render_search2(
    moduledata_t ** mlist,
    int mlistsize,
    regex_t * reg
    )
{
    int i;
    moduledata_t * mitem;
    for(i=0;i<mlistsize;i++) {
        if( module_filter( reg , mlist[i] ) == 0 )
            continue;
        mitem = mlist[i];
        printf( "%-40s - %-10s (%s)\n" , mitem->name , mitem->version , mitem->path );
    }
}

/*
   show long module url list
 * */
void
render_search3(
    moduledata_t ** mlist,
    int mlistsize,
    regex_t * reg,
    char * hosturl
    )
{
    int i;
    moduledata_t * mitem;

    char longurl[300];
    for(i=0;i<mlistsize;i++) {
        if( module_filter( reg , mlist[i] ) == 0 )
            continue;
        mitem = mlist[i];
        sprintf( longurl , "%s/authors/id/%s" , hosturl , mitem->path );
        printf( "%-40s - %-10s (%s)\n" , mitem->name , mitem->version , longurl );
    }
}


int smeta_read( FILE * in , sourcemeta_t * smeta )
{
    fread( smeta , sizeof(sourcemeta_t) , 1 , in );
    // XXX: check version and compatible here...
    // printf("meta version: %f\n",smeta->version);
    // printf("module num:   %d\n",smeta->modulenum);
    // printf("source list url: %s\n" , smeta->uri );
    return 0;
}


moduledata_t ** modulelist_new( int size )
{
    int i;
    moduledata_t ** mlist;
    mlist = (moduledata_t **) malloc (sizeof (moduledata_t*) * size);
    for (i = 0; i < size; i++)
        mlist[i] = (moduledata_t *) malloc (sizeof (moduledata_t));
    return mlist;
}

void modulelist_free( moduledata_t ** mlist , int size )
{
    int i;
    for (i = 0; i < size; i++)
        free (mlist[i]);
    free(mlist);
}

void modulelist_read( moduledata_t ** mlist , int num , FILE * in ) 
{
    int i = 0;
    while( !feof(in) && i < num )
    {
        // printf( "%d\r" , i );
        memset( mlist[i] , 0 , sizeof(moduledata_t) );
        fread( mlist[i] , sizeof(moduledata_t) , 1 , in );
        i++;
    }
}



void reg_compile( regex_t * reg , const char * pattern )
{
    int regflag;
    regflag = REG_NOSUB | REG_EXTENDED;
    if (ignore_case)
        regflag = regflag | REG_ICASE;
    assert (regcomp (reg, pattern, regflag) == 0);
}



int search(const char * pattern)
{

    sourcemeta_t smeta;
    FILE *      in;

    regex_t     reg;

    moduledata_t ** mlist;
    int mlistsize;

    // compile regular expression
    reg_compile( &reg , pattern );


    in = fopen ( indexfile() , "rb+");
    assert( in != NULL );
    smeta_read( in , &smeta );

    mlistsize = smeta.modulenum;
    mlist = modulelist_new( mlistsize );
    modulelist_read( mlist, mlistsize , in );
    fclose(in);

    switch (verbose) 
    {
        case 1:
            render_search2( mlist , mlistsize , &reg );
            break;

        case 2:
            {
                char hosturl[128] = {0} ;
                char * url = smeta.uri;
                char * c;
                c = strstr( url , "/modules" );
                strncpy( hosturl , url , c - url ); 
                render_search3( mlist , mlistsize , &reg , hosturl );
            }
            break;

        case 0:
        default:
            render_search( mlist , mlistsize , &reg );
    }
    modulelist_free( mlist , mlistsize );
    return 0;
}


