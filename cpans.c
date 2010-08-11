#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>
#include <unistd.h>


typedef struct {
    void * buffer;
    int  index;
    int  length;
} membuf;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
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



void _gunzip( char * file )
{
    char cmd[64];
    sprintf( cmd , "gunzip %s" , file );
    system( cmd );
}

int init( char * mirror_site )
{
    membuf * mbuf = (membuf*) malloc( sizeof(membuf) );
    mbuf->index  = 0;
    mbuf->length = 1024 * 24;
    mbuf->buffer = malloc( sizeof(char) * 1024 * 24 );
    memset( mbuf->buffer , 0 , sizeof( sizeof(char) * 1024 * 24 ) );


    char url[256];
//      strcpy( url , "http://cpan.nctu.edu.tw/index.html" );

    strcpy( url , mirror_site );
    strcat( url , "modules/02packages.details.txt.gz" );

    printf( "Downloading source from %s\n" , url );

    CURL *curl;
    CURLcode res;

    curl_global_init( CURL_GLOBAL_ALL );

    curl = curl_easy_init ();
    if (curl)
    {
        curl_easy_setopt(curl , CURLOPT_URL , url );
        curl_easy_setopt(curl , CURLOPT_WRITEFUNCTION,  write_data);
        curl_easy_setopt(curl , CURLOPT_WRITEDATA , (void *) mbuf );
        curl_easy_setopt(curl , CURLOPT_USERAGENT , "cpansearch/1.0" );
        res = curl_easy_perform (curl);
        curl_easy_cleanup (curl);
        printf("\n");
    }

    FILE * fp;
    char * tempfile = "packages.gz";
    if(mbuf->buffer) {
        fp = fopen( tempfile , "w" );
        assert( fp != NULL );

        int idx = 0;
        while( idx < mbuf->index ) {
            fwrite( mbuf->buffer + idx , 1 , 1024 , fp );
            idx += 1024;
        }
        fclose(fp);
        printf( "Source list saved.\n" );
        free(mbuf->buffer);
        free(mbuf);

        // use gunzip command to unzip the file..
        _gunzip( tempfile );
        unlink( tempfile );

        int len = strrchr( tempfile , '.' ) - tempfile;
        char outfile[32];
        strncpy( outfile , tempfile , len );
        *(outfile+len) = '\0';


        // XXX: read source and transform here


        unlink( outfile );
    }

}

int update()
{

    return 0;
}

int search(char * pattern)
{

}




int main(int argc, const char *argv[])
{
    setvbuf( stderr , 0, _IONBF, 0);
    setvbuf( stdout , 0, _IONBF, 0);

    if( argc == 3 && strcmp(argv[1],"--init") == 0  ) {
        printf( "Initializing package list from mirror\n" );
        init( "http://cpan.nctu.edu.tw/" );
    }
    else if( argc == 2 && strcmp(argv[1],"--update") == 0 ) {
        printf( "Updating package list from mirror\n" );
        // update package list

    }
    else if( argc == 2 && strcmp(argv[1],"--recent") == 0 ) {
        printf( "Searching packages from recent index\n" );
        // update package list

    }
    else if ( argc > 1 ) {
        // search


    }
    return 0;
}
