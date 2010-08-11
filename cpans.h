#ifndef __CPANS_H__
#define __CPANS_H__



typedef struct {
    char name[42];
    char version[12];
    char path[96];
} moduledata;

typedef struct {
    void * buffer;
    int  index;
    int  length;
} membuf;



#endif
