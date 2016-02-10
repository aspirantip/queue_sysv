#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <string.h>

#include <errno.h>

#include <iostream>

using namespace std;


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

    int msg_flg = IPC_CREAT | S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH;
    int msg_id = msgget(key, msg_flg);
    if (msg_id == -1){
        std::cout << "[error | msg_id] " << strerror( errno ) << std::endl;
        return 0;
    }
    else
        std::cout << "message queue identifier = " << msg_id << std::endl;


    // write message to file
    // ======================================
    char *buff;
    int st_msgrcv = msgrcv(msg_id, buff, 80, 0, 0);
    if (st_msgrcv){
        std::cout << "[error | st_msgrcv] " << strerror( errno ) << std::endl;
        return 0;
    }

    printf("Text = %s", buff);
    FILE * pFile;
    pFile = fopen ("/home/box/message.txt","w");
    if ( pFile != NULL )
    {
        fputs (buff,pFile);
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
