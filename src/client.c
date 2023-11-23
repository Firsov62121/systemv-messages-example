#include "shared_data.h"

int main(void)
{
    key_t server_queue_key;
    int server_qid, myqid;
    struct message my_message, return_message;

    // create my client queue for receiving messages from server
    if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1) {
        perror ("msgget: myqid");
        exit (1);
    }

    if ((server_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) {
        perror ("ftok");
        exit (1);
    }

    if ((server_qid = msgget (server_queue_key, 0)) == -1) {
        perror ("msgget: server_qid");
        exit (1);
    }

    my_message.message_type = 1;
    my_message.message_text.qid = myqid;

    printf ("Client started!\n");

    strcpy(my_message.message_text.buf, "Ping!");
    // send message to server
    if (msgsnd (server_qid, &my_message, sizeof (struct message_text), 0) == -1) {
        perror ("client: msgsnd");
        exit (1);
    }

    // read response from server
    if (msgrcv (myqid, &return_message, sizeof (struct message_text), 0, 0) == -1) {
        perror ("client: msgrcv");
        exit (1);
    }

    // process return message from server
    printf ("Message received from server: \"%s\"\n", return_message.message_text.buf);  

    // remove message queue
    if (msgctl (myqid, IPC_RMID, NULL) == -1) {
        perror ("client: msgctl");
        exit (1);
    }
    printf ("Client finished!\n");

    exit (0);
}
