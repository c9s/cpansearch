

PROG=cpans
SRCS=cpans.c
OBJS=$(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -Wall -Werror $(OBJS) -o $(PROG)

clean:	
	rm -rf $(PROG) $(OBJS)
