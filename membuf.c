#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>
#include "membuf.h"

size_t _write_data (void *buffer, size_t size, size_t nmemb, void *userp) {

    // printf(". %d %d\n", size , nmemb);
    printf( "." );

    membuf * mbuf = (membuf*) userp;

    if( mbuf->index + nmemb > mbuf->length ) {
        mbuf->length += 1024 * 1000;
        mbuf->buffer = (void*) realloc( mbuf->buffer , mbuf->length * sizeof(char) );
    }

    memcpy( mbuf->buffer + mbuf->index , buffer,
            nmemb * size );
    mbuf->index += nmemb * size;
    return nmemb;
}


membuf * membuf_new () {

    membuf * mbuf = (membuf*) malloc( sizeof(membuf) );
    mbuf->index  = 0;
    mbuf->length = 1024 * 24;
    mbuf->buffer = malloc( sizeof(char) * 1024 * 24 );
    memset( mbuf->buffer , 0 , sizeof( sizeof(char) * 1024 * 24 ) );
    return mbuf;

}


/* return source list file path */
membuf * membuf_curl ( const char * url ) {

    membuf * mbuf = membuf_new();

    CURL *curl;
    CURLcode res;

    curl_global_init( CURL_GLOBAL_ALL );

    curl = curl_easy_init ();
    if (curl)
    {
        curl_easy_setopt(curl , CURLOPT_URL , url );
        curl_easy_setopt(curl , CURLOPT_WRITEFUNCTION,  _write_data);
        curl_easy_setopt(curl , CURLOPT_WRITEDATA , (void *) mbuf );
        curl_easy_setopt(curl , CURLOPT_USERAGENT , "cpansearch/1.0" );
        res = curl_easy_perform (curl);
        curl_easy_cleanup (curl);
    }
    return mbuf;
}



void membuf_free ( membuf * mbuf ) {

    free( mbuf->buffer );
    free( mbuf );

}

void membuf_writefile( membuf * mbuf , const char * file ) {

    FILE *fp;
    int idx = 0;

    fp = fopen( file , "wb" );
    assert( fp != NULL );

    while( idx < mbuf->index ) {
        fwrite( mbuf->buffer + idx , 1 , 1024 , fp );
        idx += 1024;
    }
    fclose(fp);
}
