#include <stdio.h>

void help()
{
    printf( "cpans usage:\n\n" );
    printf( "  To init source list:\n\n" );
    printf( "     cpans -i http://cpan.nctu.edu.tw/ \n" );
    printf( "     cpans --init http://cpan.nctu.edu.tw/ \n\n" );

    printf( "  To update source list:\n\n" );
    printf( "     cpans --update  \n" );
    printf( "     cpans --u  \n\n" );

    printf( "  To search:\n\n" );
    printf( "     cpans [pattern]  \n\n" );
}
