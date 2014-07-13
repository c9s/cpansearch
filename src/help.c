#include <stdio.h>

void help () {

    printf( "cpans usage:\n\n"
            "  To init source list:\n\n"
            "     cpans -f http://cpan.nctu.edu.tw/ \n"
            "     cpans --fetch http://cpan.nctu.edu.tw/ \n\n"

            "  To update source list:\n\n"
            "     cpans --update  \n"
            "     cpans --u  \n\n"

            "  To search:\n\n"
            "     cpans [pattern]  \n\n" 

            "  To enable ncurse mode:\n\n"
            "     cpans --nc [pattern]  \n\n" 
            );

}
