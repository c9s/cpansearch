
PROG=cpans

# CFLAGS= -I. -Iinclude/ -Wall -Werror -std=c99 -g $$(pkg-config --cflags glib-2.0 libcurl)
# CFLAGS= -I. -Iinclude/ -Wall -std=c99 -g $$(pkg-config --cflags glib-2.0 libcurl) -D_GNU_SOURCE
CFLAGS= -I. -Iinclude/ -Wall -g $$(pkg-config --cflags glib-2.0 libcurl)

CFLAGS+= -lmenu -lncurses 

LDFLAGS= $$(pkg-config --libs  glib-2.0 libcurl)

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
		@echo "   cpans -f http://cpan.perl.org/"
		@echo "----------------"

install: $(PROG)
		./cpans --fetch http://cpan.perl.org/
		mkdir -p    /usr/bin
		cp -v $(PROG) /usr/bin

test:
		true

clean:  
		rm -rvf $(PROG) $(OBJS)
