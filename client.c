#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 256

// 시그널 핸들러 함수 정의
void sigint_handler(int sig) {
    
    (void)sig;
    
    printf("\nCtrl+C detected. Printing server log...\n");

    //server_log.txt 파일 출력 
    execl("/bin/cat", "cat", "/home/sumin/project24/server_log.txt", NULL);

    // execl 함수는 실행에 실패할 경우 perror출력.
    perror("execl");
    exit(EXIT_FAILURE);
}

int main() {
    // 시그널 핸들러 등록
    signal(SIGINT, sigint_handler);

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == -1) {
        perror("inet_pton");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);

        ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("send");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("recv");
            close(client_socket);
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0) {
            printf("Server closed connection\n");
            break;
        }

        printf("Server: %s\n", buffer);

        // Check if the user wants to quit
        if (strcmp(buffer, "quit\n") == 0) {
            printf("Quitting...\n");
            exit(EXIT_SUCCESS);
        }
    }

    close(client_socket);

    return 0;
}
