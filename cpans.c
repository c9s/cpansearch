#include <stdio.h>
#include <stdlib.h>

int init()
{

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
    if( argc == 3 && strcmp(argv[1],"--init") == 0  ) {
        printf( "Initializing package list from mirror\n" );
        // initialize mirror 
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
