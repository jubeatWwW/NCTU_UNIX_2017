#include "client.h"

client::client(const char* ip, int portno){
    this->ip = strdup(ip);
    this->portno = portno;
    this->restart = false;
    sockConf();
}

void client::sockConf(){
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("error opening socket\n");
        exit(1);
    }

    bzero((char*) &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_ntop(AF_INET, &(servaddr.sin_addr), ip, INET_ADDRSTRLEN);
    servaddr.sin_port = htons(portno);
}

void client::Connect(){
    if(connect(sockfd, (sockaddr*) &servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        exit(1);
    }
}

void client::Close(){
    close(sockfd);
}

void client::cThread(client* c){
    printf("this is client thread\n");
    c->Connect();

restart:
    thread r(readThread, c->sockfd);
    thread w(screenThread, c->sockfd);
    r.join();
    if(c->restart){
        w.detach();
        c->restart = false;
        goto restart;
    } else {
    	endwin();
        w.detach();
        c->Close();
    }
}
