CC = gcc
NAME = demo
LIB = diffboot
DFLIB = -l$(LIB) -L./
CFLAGS = $(shell curl-config --cflags) $(shell pkg-config --cflags json)
LDFLAGS = $(shell curl-config --libs) $(shell pkg-config --libs json) $(DFLIB)

INCLUDE = -I./
SRC = ./

all: demo lib

demo: $(NAME)
$(NAME): main.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) main.c -o $(NAME)

lib: $(LIB)
$(LIB): DBCClient.o
	gcc -shared $(CFLAGS)  $(LDFLAGS) DBCClient.o -o $(LIB).dlyb
DBCClient.o: DBCClient.c
	gcc -fPIC -c DBCClient.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(NAME)
