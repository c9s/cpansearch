#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>
#include <unistd.h>

#include <stdarg.h>
#include <sys/types.h>
#include <regex.h>
#include <glib.h>

#include "cpans.h"

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


char * skipspace( char * s2 ) 
{
    while( *s2 != ' ' && *s2 != '\0' && *s2 != '\n' ) s2++;
    return s2;
}

void cpansearch_datafile(char * path )
{
    sprintf( path , "%s/.cpansearch.dat" , g_get_home_dir() );
}

/* return source list file path */
membuf * membuf_curl( const char * url )
{
    membuf * mbuf = (membuf*) malloc( sizeof(membuf) );
    mbuf->index  = 0;
    mbuf->length = 1024 * 24;
    mbuf->buffer = malloc( sizeof(char) * 1024 * 24 );
    memset( mbuf->buffer , 0 , sizeof( sizeof(char) * 1024 * 24 ) );

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
    }
    return mbuf;
}

void slist_transform( const char * url , const char * sourcefile )
{
    char datafile[128];
    cpansearch_datafile( datafile );

    FILE *in = fopen( sourcefile , "r" );
    FILE *out = fopen( datafile , "w+" );

    char buffer[300];
    int i;
    for (i = 0; i < 9; i++)
        fgets( buffer , 300 , in );   // skip 9 lines (header)

    sprintf( buffer, "%s" , url );
    fwrite( buffer , 1 , 300 , out );

    while( !feof(in) ) {

        moduledata mdata;
        strcpy( mdata.name , "" );
        strcpy( mdata.version , "" );
        strcpy( mdata.path , "" );

        memset( buffer , 0 , 300 );
        fgets( buffer , 300 , in );

        char * s1, *s2;
        s1 = buffer;
        s2 = buffer;
        s2 = skipspace( s2 );
        if( s1 == s2 ) break;
        strncpy( mdata.name , s1 , s2-s1 );

        *(mdata.name + (s2-s1)) = '\0';
        while( *s2 == ' ' ) s2++;


        s1 = s2;
        s2 = s2;
        s2 = skipspace( s2 );
        if( s1 == s2 ) break;
        strncpy( mdata.version , s1 , s2-s1 );
        *(mdata.version + (s2-s1)) = '\0';
        while( *s2 == ' ' ) s2++;

        s1 = s2;
        s2 = s1;
        s2 = skipspace( s2 );
        if( s1 == s2 ) break;
        strncpy( mdata.path , s1 , s2-s1 );
        *(mdata.path + (s2-s1) ) = '\0';

        // printf( "%s - %s - %s\n" , mdata.name , mdata.version , mdata.path );
        fwrite( &mdata , sizeof(moduledata) , 1 , out  );
    }

    fclose(out);
    fclose(in);
    unlink( sourcefile );
}

char * slist_url()
{
    FILE * in;
    char datafile[128];
    char url[300];

    cpansearch_datafile( datafile );
    in = fopen (datafile, "r+");
    assert( in != NULL );

    fread( url , 1 , 300 , in );
    printf( "Source list from: %s\n" , url );
    fclose( in );
    return strdup(url);
}




void membuf_free( membuf * mbuf )
{
    free( mbuf->buffer );
    free( mbuf );
}

void membuf_writefile( membuf * mbuf , const char * file )
{
    FILE *fp;
    int idx = 0;

    fp = fopen( file , "w" );
    assert( fp != NULL );

    while( idx < mbuf->index ) {
        fwrite( mbuf->buffer + idx , 1 , 1024 , fp );
        idx += 1024;
    }
    fclose(fp);
}


void _gunzip( char * file )
{
    char cmd[32];
    sprintf( cmd , "gunzip -f %s" , file );
    system( cmd );
}

int init( const char * mirror_site )
{
    char url[256];
    membuf * mbuf;
    strcpy( url , mirror_site );
    strcat( url , "modules/02packages.details.txt.gz" );

    printf( "Downloading source from %s\n" , url );
    mbuf = membuf_curl( url );

    char * tempfile = "packages.gz";
    assert( mbuf->buffer != NULL );
    membuf_writefile( mbuf , tempfile );
    membuf_free( mbuf );
    printf( "Source list saved.\n" );

    // use gunzip command to unzip the file..
    _gunzip( tempfile );
    unlink( tempfile );

    int len = strrchr( tempfile , '.' ) - tempfile;
    char outfile[32];
    strncpy( outfile , tempfile , len );
    *(outfile+len) = '\0';

    slist_transform( url , outfile );
    return 0;
}



int update()
{
    char * url = slist_url();

    membuf * mbuf = membuf_curl( url );
    assert( mbuf->buffer != NULL );

    char * tempfile = "packages.gz";
    membuf_writefile( mbuf , tempfile );
    membuf_free( mbuf );
    printf( "Source list saved.\n" );

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

int search(const char * pattern)
{
    char datafile[128];
    regex_t reg;
    FILE * in;
    moduledata mdata;

    cpansearch_datafile( datafile );

    in = fopen (datafile, "r+");
    assert( in != NULL );

    char url[300];
    fread( url , 1 , 300 , in );
    printf( "Source list from: %s\n" , url );

    assert( regcomp( &reg , pattern , REG_NOSUB | REG_EXTENDED ) == 0 );

    regmatch_t matchlist[1];

    while( !feof(in) ) {
        memset( &mdata , 0 , sizeof(moduledata) );
        fread( &mdata , sizeof(moduledata) , 1 , in );

        if( regexec( &reg , mdata.name  , 1 , matchlist , 0 ) == 0 ) {
            printf( "%s - %s\n" , mdata.name , mdata.version );
        }
    }
    fclose(in);
    return 0;
}




int main(int argc, const char *argv[])
{
    setvbuf( stderr , 0, _IONBF, 0);
    setvbuf( stdout , 0, _IONBF, 0);

    if( argc == 3 && strcmp(argv[1],"--init") == 0  ) {
        printf( "Initializing package list from mirror\n" );
        init( argv[2] );
    }
    else if( argc == 2 && strcmp(argv[1],"--update") == 0 ) {
        printf( "Updating package list from mirror\n" );
        // update package list
        update();
    }
    else if( argc == 2 && strcmp(argv[1],"--recent") == 0 ) {
        printf( "Searching packages from recent index\n" );
        // update package list

    }
    else if ( argc > 1 ) {
        // search
        search( argv[1] );
    }
    return 0;
}
