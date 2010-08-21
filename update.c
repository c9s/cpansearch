#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "cpans.h"
#include "membuf.h"
#include "utils.h"


int update () {

    char * url = slist_url();

    membuf * mbuf = membuf_curl( url );
    assert( mbuf->buffer != NULL );

    char * tempfile = "packages.gz";
    membuf_writefile( mbuf , tempfile );
    printf( "\nSource list saved.\n" );

    _gunzip( tempfile );
    unlink( tempfile );

    int len = strrchr( tempfile , '.' ) - tempfile;
    char outfile[32];
    strncpy( outfile , tempfile , len );
    *(outfile+len) = '\0';

    slist_transform( url , outfile );

    membuf_free( mbuf );
    free(url);
    return 0;
}
