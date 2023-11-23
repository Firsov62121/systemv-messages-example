#include "shared_data.h"

int main(void) {
    key_t msg_queue_key;
    int qid;
    struct message message;

    if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf ("Server started!\n");

    while (1) {
        // read an incoming message
        if (msgrcv(qid, &message, sizeof (struct message_text), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Server received massage: \"%s\".\n", message.message_text.buf);

        // process message
        // int length = strlen(message.message_text.buf);
        // char buf [20];
        // sprintf(buf, " %d", length);
        strcpy(message.message_text.buf, "Pong!");

        int client_qid = message.message_text.qid;
        message.message_text.qid = qid;

        // send reply message to client
        if (msgsnd(client_qid, &message, sizeof (struct message_text), 0) == -1) {  
            perror("msgget");
            exit(1);
        }

        printf("Server sent response \"Pong!\" to client.\n");
    }
    return 0;
}