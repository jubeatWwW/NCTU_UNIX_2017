#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

using namespace std;

#include "server.h"
#include "client.h"
#include "Game.h"

server *s;
client *c;
Game *game;
int role;
int curPlayer = PLAYER1;

void serverThread();
void clientThread();
void readThread(int fd);
void writeThread(int fd);
void screenThread(int fd);

int main(int argc, char *argv[]){
    if(3 != argc){
        printf("usage: \n");
        exit(1);
    }

    string opt(argv[1]);
    
    int portno;
    

    if("-s" == opt){
        role = PLAYER1;
        s = new server(atoi(argv[2]));
        server::sThread(s);
    } else if("-c" == opt) {
        role = PLAYER2;
        printf("act as client\n");
        string ipstr(argv[2]);

        size_t fnum = ipstr.find(':');
        if(string::npos == fnum){
            printf("ip format error\n");
            exit(1);
        }
        
        portno = atoi(ipstr.substr(fnum+1).c_str());
        char* ip = strdup(ipstr.substr(0, fnum).c_str());
        printf("ip: %s\n", ip);
        printf("port: %d\n", portno);

        c = new client(ip, portno);
        client::cThread(c);

    } else {
        printf("Unknown option\n");
    }
}

void readThread(int fd){
    
    char buffer[256];
    int n;
    while(true){
        if((n = read(fd, buffer, 255)) < 0){
            printf("read error\n");
            exit(1);
        }
        
        if('p' == buffer[0]){
            int x = buffer[1] - '0';
            int y = buffer[2] - '0';
            draw_message("msg", 1);
            game->DropPiece(x, y);
        } else if('r' == buffer[0]){
            if(s)
                s->restart = true;
            if(c)
                c->restart = true;
            break;
        } else if('q' == buffer[0]){
            break;
        }
    }

}

void writeThread(int fd){
    
    char buffer[256];
    int n;
    while(true){
        printf("input: ");
        scanf("%s", buffer);
        if((n = write(fd, buffer, strlen(buffer))) < 0){
            printf("write error\n");
            exit(1);
        }
    }

}

void screenThread(int fd){
    game = new Game(fd, role);
    while(game->controller());
	endwin();			// end curses mode
    exit(0);
}
