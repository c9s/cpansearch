#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <glib.h>
#include "cpans.h"
#include "membuf.h"

char * indexfile() {
    /* return ~/.cpansearch.dat */
    char *datafile = malloc(256*sizeof(char));
    sprintf( datafile, "%s/.cpansearch.dat", g_get_home_dir() );
    return datafile;
}

char * skipword( char * s2 ) {

    while( isgraph(*s2) ) { s2++; }
    return s2;

}

char * skipspace( char * s2 ) {

    while( *s2 == ' ' ) { s2++; }
    return s2;

}

void _gunzip( char * file ) {

    char cmd[32];
    sprintf( cmd , "gunzip -f %s" , file );
    system( cmd );

}

char * slist_url() {

    FILE * in = fopen ( indexfile(), "rb+");
    assert( in != NULL );

    sourcemeta_t * smeta = malloc(sizeof(sourcemeta_t));
    fread( smeta , sizeof(sourcemeta_t) , 1 , in );
    printf( "Source list from: %s\n" , smeta->uri );
    fclose( in );
    return smeta->uri;

}

void slist_transform( const char * url , const char * sourcefile ) {

    sourcemeta_t smeta;

    FILE *in = fopen( sourcefile , "rb" );
    FILE *out = fopen( indexfile() , "wb+" );

    char buffer[300];
    for (int i = 0; i < 9; i++) {
        fgets( buffer , 300 , in );   // skip 9 lines (header)
    }

    memset( &smeta , 0 , sizeof( sourcemeta_t ));
    sprintf( smeta.uri , "%s" , url );
    fwrite( &smeta , sizeof(sourcemeta_t) , 1 , out );


    int cnt = 0;
    char * s1, *s2;
    while( !feof(in) ) {

        moduledata_t mdata;
        memset( buffer , 0 , 300 );
        memset( &mdata , 0 , sizeof(moduledata_t) );
        fgets( buffer , 300 , in );

        if( strlen(buffer) == 0 )
            break;

        s1 = buffer;
        s2 = buffer;
        s2 = skipword( s2 );
        if( s1 == s2 ) break;
        memcpy( mdata.name , s1 , (s2-s1));
        s2 = skipspace( s2 );

        s1 = s2;
        s2 = s2;
        s2 = skipword( s2 );
        if( s1 == s2 ) break;
        memcpy( mdata.version , s1 , (s2-s1) );
        if( strcmp(mdata.version ,"undef") == 0 )
            strcpy(mdata.version,"0");
        s2 = skipspace( s2 );

        s1 = s2;
        s2 = s1;
        s2 = skipword( s2 );
        if( s1 == s2 ) break;
        memcpy( mdata.path , s1 , (s2-s1) );

        // printf( "%s - %s - %s\n" , mdata.name , mdata.version , mdata.path );
        fwrite( &mdata , sizeof(moduledata_t) , 1 , out  );
        cnt++;
    }

    printf( "%d packages recorded.\n" , cnt );

    fclose(out);
    fclose(in);
    unlink( sourcefile );
}

