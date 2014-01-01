
PROG=cpans
DEFAULT_MIRROR=http://www.cpan.org/

CFLAGS=  -Wall -O3 -Iinclude $$(pkg-config --static --cflags glib-2.0)
LDFLAGS= $$(pkg-config --static --libs glib-2.0) -lmenu -lncurses -lcurl

SRCS=src/cpans.c \
	src/membuf.c \
	src/help.c \
	src/utils.c \
	src/search.c \
	src/init.c \
	src/update.c \
	src/ncurses.c

OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
		$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(PROG)
		@echo "----------------"
		@echo "When you upgrade cpans, please update the source data again with -f option:"
		@echo "   cpans -f $(DEFAULT_MIRROR)"
		@echo "----------------"

install: $(PROG)
		./cpans --fetch $(DEFAULT_MIRROR)
		mkdir -p /usr/bin
		cp -v $(PROG) /usr/bin

test:
		true

clean:  
		rm -rvf $(PROG) $(OBJS)
