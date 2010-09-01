#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include <glib.h>

#include "nc.h"
#include "cpans.h"

// XXX: 
#define ARRAY_SIZE(a) ( 100 )
#define CTRLD 	4

char **choices;

ITEM **cpans_menu_items;
MENU *cpans_menu;
ITEM *cur_item;


char * modulename_path( const char * hosturl , char * name )
{
    char path[300] = {0};

    strcat( path , hosturl );
    strcat( path , "/dist/" );


    char * c;
    c = name;
    int i;
    int j = strlen(path);
    for (i = 0; i < strlen (name); i++) {
        if( name[i] == ':' && name[i+1] == ':' ) {
            i++;
            path[j++] = '-';
        }
        else {
            path[j++] = name[i];
        }
    }
    return strdup( path );
}


char * find_cpan_prog()
{
    gchar * path;
    path = g_find_program_in_path("cpanm");
    if( path != NULL )
        return path;

    path = g_find_program_in_path("cpanp");
    if( path != NULL )
        return path;

    path = g_find_program_in_path("cpan");
    return path;
}


void cpans_nc_init( moduledata_t ** mlist , size_t mlistsize )
{
    int i;

    /* Initialize curses */
    initscr ();
    cbreak ();
    noecho ();
    keypad (stdscr, TRUE);


    /* Initialize items */
    cpans_menu_items = (ITEM **) calloc ( mlistsize + 1, sizeof (ITEM *));
    for (i = 0; i < mlistsize; ++i)
        cpans_menu_items[i] = new_item( 
                    strdup(mlist[i]->name) , 
                    strdup( mlist[i]->version )
                );

    cpans_menu_items[ mlistsize ] = (ITEM *) NULL;

    cpans_menu = new_menu ((ITEM **) cpans_menu_items);

    set_menu_format( cpans_menu , LINES - 6 , 1 );
    set_menu_mark(   cpans_menu , " * ");
    set_menu_pad(    cpans_menu , 10 );
    set_menu_spacing(    cpans_menu , 5 , 0 , 0 );

    /* Make the menu multi valued */
    menu_opts_off( cpans_menu, O_ONEVALUE);

    mvprintw (LINES - 3, 0, "<SPACE>: select item.  <q>: quit. <j/k>: move cursor.");
    mvprintw (LINES - 2, 0, "<ENTER> to install. <p>: perldoc. <b>: browse on search.cpan.org");
    post_menu (cpans_menu);
    refresh ();
}

void cpans_nc_loop()
{

    int i;
    int c;
    while ((c = getch ()) != 'q' )
    {
        switch (c)
        {

        case 'j':
        case KEY_DOWN:
            menu_driver(cpans_menu, REQ_DOWN_ITEM);
            break;

        case 'k':
        case KEY_UP:
            menu_driver(cpans_menu, REQ_UP_ITEM);
            break;

        // toggle item
        case ' ':
            menu_driver(cpans_menu, REQ_TOGGLE_ITEM);
            break;

        case 'b':
            {
                ITEM *cur;
				cur = current_item(cpans_menu);
                char * name = item_name(cur);

                char * url  = modulename_path( "http://search.cpan.org" , name );
                // mvprintw (LINES - 3, 0, url );

                gchar * prog;
                prog = g_find_program_in_path("google-chrome");
                if( prog == NULL )
                    prog = g_find_program_in_path("firefox");
                if( prog == NULL )
                    break;

                char * name_t;

                int status;
                pid_t pid = fork();
                if( pid == 0 ) {
                    erase();
                    refresh();
                    execl( prog , "" , url , NULL );
                    exit(0);
                }
                waitpid( pid , &status, 0 );
                free( prog );
                redrawwin( stdscr );
            }
            break;

        // launch perldoc
        case 'p':
            {
                ITEM *cur;
				cur = current_item(cpans_menu);
                char * name = item_name(cur);
                gchar * prog = g_find_program_in_path("perldoc");
                int status;
                pid_t pid = fork();
                if( pid == 0 ) {
                    erase();
                    refresh();
                    execl( prog , "" , name , NULL );
                    exit(0);
                }
                waitpid( pid , &status, 0 );
                free( prog );
                redrawwin( stdscr );
            }
            break;

        case 10:               /* Enter */
            {
                // scan selected items
                char temp[200];
                ITEM **items;
                items = menu_items (cpans_menu);
                temp[0] = '\0';

                /*
                move (0, 0);
                strcat( temp , "Now cpans will install:\n" );
                for (i = 0; i < item_count (cpans_menu); ++i) {
                    if (item_value(items[i]) != TRUE)
                        continue;
                    strcat (temp, item_name (items[i]));
                    strcat (temp, " ");
                }
                strcat( temp, "\n" );
                mvprintw( 0 , 0, temp);
                */
                int j = 0;
                char ** nlist = (char**) malloc( sizeof(char*) * item_count(cpans_menu) );

                for (i = 0; i < item_count (cpans_menu); ++i) {
                    if (item_value (items[i]) != TRUE)
                        continue;
                    nlist[j++] = strdup( item_name( items[i] ) );
                }
                // XXX: end ncurses menu and window to prevent screen output breaking...
                cpans_nc_end();

                if( j == 0 ) 
                    return;

                setvbuf( stderr , 0, _IONBF, 0);
                setvbuf( stdout , 0, _IONBF, 0);

                char * prog = find_cpan_prog();
                printf( "Found cpan program: %s\n" , prog );

                printf( "Now Will Install: \n   " );
                for(i = 0; i < j; ++i ) {
                    printf( "%s " , nlist[i] );
                }
                printf( "\n" );

                int status;
                pid_t pid;
                for(i = 0; i < j; ++i ) {
                    pid = fork();
                    if( pid == 0 ) {
                        printf( "* Running %s --sudo %s\n" , prog , nlist[i] );
                        execl( prog , "" , "--sudo" , nlist[i] , 0 );
                        exit(0);
                    }
                    waitpid( pid , &status, 0 );
                }
                free( prog );
                printf( "Done\n" );
                return;
            }
            break;
        }
    }
    cpans_nc_end();
}

void cpans_nc_end() 
{ 
    free_item (cpans_menu_items[0]);
    free_item (cpans_menu_items[1]);
    free_menu (cpans_menu);
    endwin ();
}

