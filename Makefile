

PROG=cpans
CFLAGS= -Wall -Werror -std=c99 -g $$(pkg-config --cflags glib-2.0 libcurl)
LDFLAGS= $$(pkg-config --libs  glib-2.0 libcurl)
SRCS= cpans.c membuf.c help.c search.c utils.c
OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(PROG)
	@echo "----------------"
	@echo "When you upgrade cpans, please update the source data again with -i option:"
	@echo "   cpans -i http://search.cpan.org/"
	@echo "----------------"

install: $(PROG)
	mkdir -p /usr/bin/
	cp $(PROG) /usr/bin/

clean:	
	rm -rf $(PROG) $(OBJS)
