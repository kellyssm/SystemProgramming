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

    // 메시지 큐에 접근
    msgid = msgget(QUEUE_KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // 메시지 큐에서 메시지 수신 (타입이 1인 메시지 수신)
    if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Received message: %s\n", msg.msg_text);

    return 0;
}
