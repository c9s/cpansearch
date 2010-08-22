#include <stdio.h>
#include <getopt.h>

#include "cpans.h"
#include "membuf.h"
#include "help.h"
#include "search.h"
#include "utils.h"
#include "init.h"
#include "update.h"

char version[] = "0.2";
char ignore_case = 0;
char fullurl     = 0;
char nameonly    = 0;


void print_version()
{
    printf("cpans -- version %s\n", version );
}

/* getopt setting start */
static int option_index = 0;
static int thisopt = 0;

static struct option long_options[] = {
  { "fetch"     , required_argument, 0 , 'f' },
  { "init"      , required_argument, 0 , 'f' },
  { "update"    , no_argument      , 0 , 'u' },
  { "recent"    , no_argument      , 0 , 'r' },
  { "help"      , no_argument      , 0 , 'h' },
  { "url"       , no_argument      , 0 , 'l' },
  { "name"      , no_argument      , 0 , 'n' },
  { "version"      , no_argument   , 0 , 'v' },
};
/* getopt setting end */

int main(int argc, char **argv)
{

    setvbuf( stderr , 0, _IONBF, 0);
    setvbuf( stdout , 0, _IONBF, 0);

    int optbind = 0;
    while( (thisopt = getopt_long(argc, argv, "vif:urhnl", long_options, &option_index)) != -1 ) {

      switch (thisopt) {
        case 'v':
            print_version();
            return 0;

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
          break;

        case 'l':
          fullurl = 1;
          ++optbind;
          break;

        case 'n':
          nameonly = 1;
          ++optbind;
          break;

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



