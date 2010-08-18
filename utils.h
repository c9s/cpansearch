#ifndef __UTILS_H__
#define __UTILS_H__

char * skipword( char * s2 ) {

    while( isgraph(*s2) ) { s2++; }
    return s2;

}

char * skipspace( char * s2 ) {

    while( *s2 == ' ' ) { s2++; }
    return s2;

}

void _gunzip( char * file ) {

    char cmd[32];
    sprintf( cmd , "gunzip -f %s" , file );
    system( cmd );

}

char * slist_url() {

    FILE * in = fopen ( indexfile(), "rb+");
    assert( in != NULL );

    sourcemeta_t smeta;
    fread( &smeta , sizeof(sourcemeta_t) , 1 , in );
    printf( "Source list from: %s\n" , smeta.uri );
    fclose( in );
    return strdup(smeta.uri);

}


#endif
