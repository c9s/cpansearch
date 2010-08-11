

PROG=cpans
SRCS=cpans.c
CFLAGS=$$(pkg-config --cflags glib-2.0)
LDFLAGS=
OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wall -Werror $(OBJS) -o $(PROG)

clean:	
	rm -rf $(PROG) $(OBJS)
