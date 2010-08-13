#ifndef __CPANS_H__
#define __CPANS_H__

typedef struct {
    char listuri[80];
    float version;
    time_t updatetime;
} sourcemeta_t;

typedef struct {
    char name[60];
    char version[12];
    char path[200];
} moduledata_t;

#endif
