#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>

#include "cpans.h"
#include "membuf.h"
#include "utils.h"

int init( char * mirror_site ) {

    char url[256];
    membuf * mbuf;

    // check mirror_site url , should be end with "/"
    if( * (mirror_site + strlen(mirror_site) - 1 ) != '/' )
        strcat(mirror_site , "/");

    sprintf (url, "%s%s", mirror_site, "modules/02packages.details.txt.gz");

    fprintf( stderr, "Downloading source from %s\n" , url );
    mbuf = membuf_curl( url );

    char * tempfile = "packages.gz";
    assert( mbuf->buffer != NULL );
    membuf_writefile( mbuf , tempfile );
    membuf_free( mbuf );

    fprintf( stderr, "\nSource list saved.\n" );

    // use gunzip command to unzip the file..
    _gunzip( tempfile );
    unlink( tempfile );

    int len = strrchr( tempfile , '.' ) - tempfile;
    char outfile[32];
    strncpy( outfile , tempfile , len );
    *(outfile+len) = '\0';

    fprintf( stderr, "Transforming source list format.\n" );
    slist_transform( url , outfile );

    fprintf( stderr, "Done\n" );
    return 0;
}

void init_local ( char * localpath ) {


}

char *  find_minicpanrc()
{
    char *rc = (char*) malloc(  sizeof(char) * 100 );
    char *home = getenv("HOME");
    sprintf( rc , "%s/.minicpanrc" , home );
    if (!file_test (rc))
        return NULL;
    return rc;
}

void init_from_minicpanrc ( char * minicpanrc ) {
    FILE *rc = fopen( minicpanrc , "r" );
    char buffer[50] = {0};
    char localpath[64] = {0};

    strcat( localpath , "file://" );

    while(!feof(rc)){
        fgets( buffer , 50 , rc );
        if( strstr(buffer,"local:") == buffer ) {
            char * c = strchr(buffer,'/');
            strcat( localpath , c );

            // chomp
            *(localpath + strlen(localpath) - 1) = '\0';
            break;
        }
    }
    fclose(rc);
    assert( localpath != NULL );

    if( * (localpath + strlen(localpath) - 1 ) != '/' )
        strcat(localpath , "/");

    printf( "Found minicpanrc: %s\n" , localpath );
    init( localpath );
}
