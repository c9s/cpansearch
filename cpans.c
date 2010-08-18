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
#include "help.h"
#include "search.h"
#include "utils.h"

char version[] = "0.1";
char ignore_case = 0;
char fullurl     = 0;

int init( char * mirror_site ) {

    char url[256];
    membuf * mbuf;

    // check mirror_site url , should be end with "/"
    if( * (mirror_site + strlen(mirror_site) - 1 ) != '/' )
        strcat(mirror_site , "/");

    sprintf (url, "%s%s", mirror_site, "modules/02packages.details.txt.gz");

    printf( "Downloading source from %s\n" , url );
    mbuf = membuf_curl( url );

    char * tempfile = "packages.gz";
    assert( mbuf->buffer != NULL );
    membuf_writefile( mbuf , tempfile );
    membuf_free( mbuf );

    printf( "\nSource list saved.\n" );

    // use gunzip command to unzip the file..
    _gunzip( tempfile );
    unlink( tempfile );

    int len = strrchr( tempfile , '.' ) - tempfile;
    char outfile[32];
    strncpy( outfile , tempfile , len );
    *(outfile+len) = '\0';

    printf( "Transforming source list format.\n" );
    slist_transform( url , outfile );

    printf( "Done\n" );
    return 0;
}



int update() {

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





void init_local( char * localpath )
{



}


void init_from_minicpanrc()
{
    char minicpanrc[64];
    sprintf( minicpanrc , "%s/.minicpanrc" , g_get_home_dir() );

    if (!g_file_test (minicpanrc, G_FILE_TEST_EXISTS))
        return;



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

/* getopt setting start */
static int option_index = 0;
static int thisopt = 0;

static struct option long_options[] = {
  { "fetch"     , required_argument, 0 , 'f' },
  { "init"      , required_argument, 0 , 'f' },
  { "update"    , no_argument,      0 , 'u' },
  { "recent"    , no_argument      , 0 , 'r' },
  { "help"      , no_argument      , 0 , 'h' },
  { "url"       , no_argument      , 0 , 'l' },
};
/* getopt setting end */


int main(int argc, char **argv)
{

    setvbuf( stderr , 0, _IONBF, 0);
    setvbuf( stdout , 0, _IONBF, 0);

    int optbind = 0;
    while( (thisopt = getopt_long(argc, argv, "if:u:rh:l", long_options, &option_index)) != -1 ) {

      switch (thisopt) {

        case 'f':
          if (optarg != NULL) {
            init( (char*)optarg);
          } else {
            init_from_minicpanrc();
          }
          return 0;

        case 'i':
          ignore_case = 1;
          ++optbind;

        case 'l':
          fullurl = 1;
          ++optbind;

        case 'u':
          puts("Update package list from mirror");
          update();
          return 0;

        case 'r':
          puts("Searching packages from recent index");
          // update package list
          return 0;

        case 'h':
          help();
          return 0;
      }

    }

    int index;
    for (index = optind; index < argc; index++) {
        search(argv[index]);
        return 0;
    }

    return 0;
}



