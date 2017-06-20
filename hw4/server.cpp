#include "server.h"

server::server(int portno){
    printf("act as server\n");
    
    this->portno = portno;
    this->restart = false;   

    sockConf();
    
}

void server::sockConf(){
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("error opening socket\n");
        exit(1);
    }

    printf("listen on port: %d\n", portno);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0 ){
        perror("bind error\n");
        exit(1);
    }

    listen(sockfd, 5);

}

void server::Accept(){
    clilen = sizeof(cliaddr);
    printf("waiting for accept\n");
    newsockfd = accept(sockfd, (sockaddr*) &cliaddr, &clilen);
}

void server::Close(){
    close(newsockfd);
    close(sockfd);
}

void server::sThread(server *s){
    printf("this is server thread\n");
    s->Accept();
restart:
    thread r(readThread, s->newsockfd);
    thread w(screenThread, s->newsockfd);
    r.join();
    if(s->restart){
        w.detach();
        s->restart = false;
        goto restart;
    } else {
    	endwin();
        w.detach();
        s->Close();
    }
}

