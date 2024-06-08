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
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <signal.h>

#define PORT 12345
#define BUFFER_SIZE 256

// 시그널 핸들러 함수 정의
void sigint_handler(int sig) {
    (void)sig;
    
    printf("\nCtrl+C detected. Printing server log...\n");

    // 서버 로그 파일 열기
    FILE *file = fopen("/home/sumin/project24/server_log.txt", "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // 서버 로그 파일 내용 출력
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // 파일 닫기
    fclose(file);

    // 시그널 핸들러 종료
    exit(EXIT_SUCCESS);
}

// 소켓을 생성하여 파일 정보 활용하여 클라이언트 요청을 처리
void handle_client(int client_socket, int client_number) {
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("recv");
            close(client_socket);
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0) {
            printf("Client %d disconnected\n", client_number);
            break;
        }

        printf("Client %d message: %s\n", client_number, buffer);

        // 파일에 메시지 로그 저장
        FILE *file = fopen("server_log.txt", "a");
        if (file == NULL) {
            perror("fopen");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "Client %d message: %s\n", client_number, buffer);
        fclose(file);


        //클라이언트에게 메시지를 응답
        ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("send");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
    }

    close(client_socket);
    exit(EXIT_SUCCESS);
}

int main() {
    // 시그널 핸들러 등록
    signal(SIGINT, sigint_handler);

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    int client_number = 0; // 클라이언트 번호 초기화

    //소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //서버 주소를 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    //소켓과 주소 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

   //클라이언트 연결 대기
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for clients...\n");

    //클라이언트의 연결 수락 및 처리
    while (1) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("accept");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        client_number++; // 새로운 클라이언트 연결될 때 마다 클라이언트 번호 증가

        printf("Client %d connected\n", client_number);

        //클라이언트 요청을 처리하기 위한 자식 프로세스 생성
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            close(client_socket);
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // 자식 프로세스
            close(server_socket);
            handle_client(client_socket, client_number);
        } else { // 부모 프로세스
            close(client_socket);
        }
    }

    close(server_socket);

    return 0;
}