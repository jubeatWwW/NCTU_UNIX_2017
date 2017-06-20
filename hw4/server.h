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
extern void screenThread(int fd);

class server{
    public:
        server(int portno);
        int portno;
        int newsockfd;
        void Accept(void);
        void Close(void);
        bool restart;
        static void sThread(server* s);
    private:   
        int sockfd;
        struct sockaddr_in servaddr, cliaddr;
        socklen_t clilen;
        void sockConf();
};
