CC = gcc
NAME = diffBootCClient
CFLAGS = -ggdb03 $(shell curl-config --cflags) $(shell pkg-config --cflags json)
LDFLAGS = $(shell curl-config --libs) $(shell pkg-config --libs json)

INCLUDE = -I./
SRC = ./

all: $(NAME)

$(NAME): main.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) main.c DBCClient.c -o $(NAME)

clean:
	rm -f $(NAME)
