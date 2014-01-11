CC = gcc
NAME = diffBootCClient
CFLAGS = -ggdb03 $(shell pkg-config --cflags json)
LDFLAGS = -lcurl $(shell pkg-config --libs json)
INCLUDE = -I./
SRC = ./

all: $(NAME)

$(NAME): main.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) main.c DBCClient.c -o $(NAME)

clean:
	rm -f $(NAME)
