#ifndef __INIT_H__
#define __INIT_H__

extern int init ( char * mirror_site );
extern void init_local ();

extern char * find_minicpanrc();
extern void init_from_minicpanrc ( char * );

#endif
