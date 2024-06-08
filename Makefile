CC = gcc
CFLAGS = -Wall -Wextra
SERVER_SRC = server.c
CLIENT_SRC = client.c
SERVER_OUT = server
CLIENT_OUT = client

# all 규칙에서 모든 실행 파일 빌드
all: $(SERVER_OUT) $(CLIENT_OUT) msg_send msg_recv hello

# server 프로그램 빌드 규칙
$(SERVER_OUT): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_OUT) $(SERVER_SRC)

# client 프로그램 빌드 규칙
$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_OUT) $(CLIENT_SRC)

# msg_send 프로그램 빌드 규칙
msg_send: msg_send.c
	$(CC) $(CFLAGS) -o msg_send msg_send.c

# msg_recv 프로그램 빌드 규칙
msg_recv: msg_recv.c
	$(CC) $(CFLAGS) -o msg_recv msg_recv.c

# hello 프로그램 빌드 규칙
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# 클린 타겟 - 생성된 파일 삭제
.PHONY: clean
clean:
	rm -f $(SERVER_OUT) $(CLIENT_OUT) msg_send msg_recv hello

# 새로 만들기 타겟 - 클린 후 다시 빌드
rebuild: clean all
