

PROG=cpans
CFLAGS=$$(pkg-config --cflags glib-2.0 libcurl)
LDFLAGS=$$(pkg-config --libs  glib-2.0 libcurl)
SRCS=cpans.c
OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wall -Werror $(OBJS) -o $(PROG)

clean:	
	rm -rf $(PROG) $(OBJS)
