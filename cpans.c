#include <stdio.h>
#include <stdlib.h>



int main(int argc, const char *argv[])
{

    if( argc == 3 && strcmp(argv[1],"--init") == 0  ) {
        // initialize mirror 
    }
    else if( argc == 2 && strcmp(argv[1],"--update") == 0 ) {
        // update package list

    }
    else if( argc == 2 && strcmp(argv[1],"--recent") == 0 ) {
        // update package list

    }
    else if ( argc > 1 ) {
        // search


    }
    return 0;
}
