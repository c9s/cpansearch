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

    set_menu_format(     cpans_menu , LINES - 5 , 1 );
    set_menu_mark(       cpans_menu , " * ");
    set_menu_pad(        cpans_menu , 3 );
    set_menu_spacing(    cpans_menu , 5 , 0 , 0 );

    /* Make the menu multi valued */
    menu_opts_off( cpans_menu, O_ONEVALUE );

    char temp[100];
    sprintf( temp , "==== Found %d packages ====" , (int) mlistsize );
    mvprintw (LINES - 5, 0, temp );

    // XXX: show a help window here.
    mvprintw (LINES - 3, 0, "<SPACE/ENTER>: select item.  <q>: quit. <j/k>: move cursor.");
    mvprintw (LINES - 2, 0, "<g> start to install. <G>: start to install in parallel. ");
    mvprintw (LINES - 1, 0, "<p>: perldoc. <b>: browse on search.cpan.org");
    post_menu (cpans_menu);
    refresh ();
}

void openweb()
{
    ITEM *cur;
    char * name;
    char * url;
    gchar * prog;
    int status;
    pid_t pid;

    cur = current_item(cpans_menu);
    name = (char*)item_name(cur);
    url  = modulename_path( "http://search.cpan.org" , name );

    prog = g_find_program_in_path("google-chrome");
    if( prog == NULL )
        prog = g_find_program_in_path("firefox");
    if( prog == NULL )
        return;

    pid = fork();
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


char ** get_selected_names(int * length)
{
    ITEM **items;
    int j,i;
    char ** nlist;

    j = 0;
    items = menu_items (cpans_menu);
    nlist = (char**) malloc( sizeof(char*) * item_count(cpans_menu) );
    for (i = 0; i < item_count (cpans_menu); ++i) {
        if (item_value (items[i]) != TRUE)
            continue;
        nlist[j++] = strdup( item_name( items[i] ) );
    }
    *length = j;
    return nlist;
}


void install_modules(char * prog, char **nlist , int len, int wait)
{
    pid_t pidlist[100];
    pid_t pid;
    int status;
    int i;

    printf( "Now Will Install: \n   " );
    for(i = 0; i < len; ++i )
        printf( "%s " , nlist[i] );
    printf( "\n" );

    for(i = 0; i < len; ++i ) {
        pid = fork();
        if( pid == 0 ) {
            printf( "* Running %s --sudo %s\n" , prog , nlist[i] );
            execl( prog , "" , "--sudo" , nlist[i] , 0 );
            exit(0);
        }
        if( wait )
            waitpid( pid , &status, 0 );
        else
            pidlist[i] = pid; // save pid to pidlist
    }

    if( ! wait ) {
        printf( "Waiting for installation finish.\n" );
        for(i = 0; i < len; ++i )
            waitpid( pidlist[i] , &status, 0 );
    }
    printf( "Installation Done\n" );
}



void openperldoc()
{
    ITEM *cur;
    cur = current_item(cpans_menu);
    char * name = (char*) item_name(cur);
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

void cpans_nc_loop()
{

    /*
     TODO: 
        add 'd' command for "Download module"
        add 'x' command for "Download and Extract module"
        add 't' or 'n' command for "Notest Install"
        add 'Ctrl-d/u' for page up/down.
        add '/' command for filtering module list.
     * */
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
        case 10:  // <Enter>
        case ' ':
            menu_driver(cpans_menu, REQ_TOGGLE_ITEM);
            break;

        case 'b':
            openweb();
            break;

        // launch perldoc
        case 'p':
            openperldoc();
            break;

        case 'G':
            {
                // scan selected items
                char temp[200];
                char ** nlist;
                int j;

                temp[0] = '\0';
                j = 0;
                nlist = get_selected_names( &j );

                // XXX: end ncurses menu and window to prevent screen output breaking...
                cpans_nc_end();

                if( j == 0 ) 
                    return;

                setvbuf( stderr , 0, _IONBF, 0);
                setvbuf( stdout , 0, _IONBF, 0);

                char * prog = find_cpan_prog();
                printf( "Found cpan program: %s\n" , prog );

                install_modules( prog , nlist , j , 0 );
                free( prog );
                return;
            }
            break;

        case 'g':               /* g */
            {
                // scan selected items
                char ** nlist;
                int j;

                j = 0;
                nlist = get_selected_names( &j );

                // XXX: end ncurses menu and window to prevent screen output breaking...
                cpans_nc_end();

                if( j == 0 ) 
                    return;

                setvbuf( stderr , 0, _IONBF, 0);
                setvbuf( stdout , 0, _IONBF, 0);

                char * prog = find_cpan_prog();
                printf( "Found cpan program: %s\n" , prog );

                install_modules( prog , nlist , j , 1 ); // wait for each installation
                free( prog );
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

