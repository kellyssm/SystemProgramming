#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define QUEUE_KEY 1234

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    int msgid;
    struct message msg;

    // 메시지 큐 생성
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    //메시지 입력 받기
    printf("Enter your message: ");
    fgets(msg.msg_text, sizeof(msg.msg_text), stdin);

    // 메시지 설정
    msg.msg_type = 1;  // 메시지 타입 설정 -> 메시지 구분

    // 메시지 큐에 메시지 송신
    if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Message sent: %s\n", msg.msg_text);
    return 0;
}