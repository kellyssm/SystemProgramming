CC = gcc
CFLAGS = -Wall -Wextra
SERVER_SRC = server.c
CLIENT_SRC = client.c
SERVER_OUT = server
CLIENT_OUT = client

all: $(SERVER_OUT) $(CLIENT_OUT) hello

$(SERVER_OUT): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_OUT) $(SERVER_SRC)

$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_OUT) $(CLIENT_SRC)

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

.PHONY: clean

clean:
	rm -f $(SERVER_OUT) $(CLIENT_OUT) hello
