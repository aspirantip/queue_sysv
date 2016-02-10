#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;

struct message{
    long mtype;
    char mtext[80];
};


int main(int argc, char **argv){
    printf("start ... \n");

    // creat queue
    // ======================================
    key_t key = ftok("/tmp/msg.temp", 1);
    printf("key = %d\n", key);
    if (key == -1){
        std::cout << "[error] " << strerror( errno ) << std::endl;
        return 0;
    }

    int msg_flg = IPC_CREAT | 0777;
    int msg_id = msgget(key, msg_flg);
    if (msg_id == -1){
        std::cout << "[error | msg_id] " << strerror( errno ) << std::endl;
        return 0;
    }
    else
        std::cout << "message queue identifier = " << msg_id << std::endl;


    // write message to file
    // ======================================

    struct message m_msg;
    int st_msgrcv = msgrcv(msg_id, (struct msgbuf *) &m_msg.mtext, 80, 0, 0);
    if (st_msgrcv == -1){
        std::cout << "[error | st_msgrcv] " << strerror( errno ) << std::endl;
        return 0;
    }

    printf("Text message = %s\n", m_msg.mtext);
    FILE * pFile;
    pFile = fopen ("/home/box/message.txt","w");
    if ( pFile != NULL )
    {
        fwrite (m_msg.mtext, sizeof(char), sizeof(st_msgrcv), pFile);
        fclose (pFile);
    }
    else{
        std::cout << "[error | write to file] " << strerror( errno ) << std::endl;
        return 0;
    }



    // close queue
    // ======================================
    int state = msgctl(msg_id, IPC_RMID, NULL);
    if (state == -1){
        std::cout << "[error | close queue] " << strerror( errno ) << std::endl;
        return 0;
    }
    else
        std::cout << "queue id = " << msg_id << " closed" << std::endl;



    printf("finished ... \n");
    return 0;
}
