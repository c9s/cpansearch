

PROG=cpans
CFLAGS=-Wall -Werror -g $$(pkg-config --cflags glib-2.0 libcurl)
LDFLAGS=$$(pkg-config --libs  glib-2.0 libcurl)
SRCS=cpans.c
OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(PROG)

clean:	
	rm -rf $(PROG) $(OBJS)
