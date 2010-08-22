#ifndef __UTILS_H__
#define __UTILS_H__

extern char * indexfile();

extern char * skipword( char * s2 );

extern char * skipspace( char * s2 );

extern void _gunzip( char * file );

extern char * slist_url();

extern void slist_transform( const char * url , const char * sourcefile );

#endif
