CC = gcc
NAME = sample
LIB = diffboot
DFLIB =-l$(LIB) -L./
CFLAGS = $(shell curl-config --cflags) $(shell pkg-config --cflags json) -DTOKEN='"4f558dfde45e6d0e1850da93954c91b1"'
LDFLAGS = $(shell curl-config --libs) $(shell pkg-config --libs json) 


INCLUDE = -I./
SRC = ./

all: lib demo 

demo: $(NAME)
$(NAME): main.c
	$(CC) main.c -ggdb03 $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(DFLIB) -o $(NAME)

lib: $(LIB)
$(LIB): DBCClient.o
	ar rcs lib$(LIB).a DBCClient.o
DBCClient.o: DBCClient.c
	gcc -fPIC -c DBCClient.c $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(NAME)
	rm -f DBCClient.o
	rm -f lib$(LIB).a
