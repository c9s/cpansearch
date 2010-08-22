#ifndef __MEMBUF_H__
#define __MEMBUF_H__

/* membuf.c */
typedef struct {
    void * buffer;
    int  index;
    int  length;
} membuf;

membuf *membuf_new(void);
membuf *membuf_curl(const char *url);
void membuf_free(membuf *mbuf);
void membuf_writefile(membuf *mbuf, const char *file);


#endif
