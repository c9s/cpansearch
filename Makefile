
PROG=cpans
DEFAULT_MIRROR=http://www.cpan.org/

# CFLAGS= -I. -Iinclude/ -Wall -Werror -std=c99 -g $$(pkg-config --cflags glib-2.0 libcurl)
# CFLAGS= -I. -Iinclude/ -Wall -std=c99 -g $$(pkg-config --cflags glib-2.0 libcurl) -D_GNU_SOURCE
CFLAGS= -O3 -I. -Iinclude/ -Wall $$(pkg-config --static --cflags glib-2.0 libcurl) 

CFLAGS+= -lmenu -lncurses 

LDFLAGS= $$(pkg-config --static --libs glib-2.0 libcurl)

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
		mkdir -p    /usr/bin
		cp -v $(PROG) /usr/bin

test:
		true

clean:  
		rm -rvf $(PROG) $(OBJS)
