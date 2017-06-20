#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;

#include "othello.h"

extern void readThread(int fd);
extern void writeThread(int fd);
extern void screenThread(int fd);

class client{
    public:
        client(const char* ip, int portno);
        char* ip;
        int portno;
        int sockfd;
        void Connect(void);
        void Close(void);
        bool restart;
        static void cThread(client* c);
    private:   
        struct sockaddr_in servaddr;
        void sockConf();
};

