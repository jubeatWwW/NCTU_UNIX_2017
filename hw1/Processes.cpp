#include "Processes.h"

Processes::Processes() : PATH("/proc"){

    socketfd = new Socketfd();
    BuildSocketTrie();
}

void Processes::BuildSocketTrie(){
    DIR *pDir;
    dirent* pDirent;
    pDir = opendir(PATH.c_str());

    while((pDirent = readdir(pDir)) != NULL){
        if(!isdigit(*pDirent->d_name))
            continue;

        //printf ("[%s]\n", pDirent->d_name);
        
        string cmdlinePath = PATH + "/" + string(pDirent->d_name) + "/cmdline";
        char cmd[1024];
        char* cmdline;
        FILE *fptr = fopen(cmdlinePath.c_str(), "r");
        if(fptr){
            while(fgets(cmd, 1024, fptr) != NULL){
                //cout << cmd;
                cmdline = strdup(cmd);
                break;
            }
        }


        string fdPath = PATH + "/" + string(pDirent->d_name) + "/fd";
        //cout << fdPath << endl;
        dirent* fdDirent;
        DIR* fdDir = opendir(fdPath.c_str());

        while((fdDirent = readdir(fdDir)) != NULL){
            if(!isdigit(*fdDirent->d_name))
                continue;
            
            string linkPath = fdPath + "/" + string(fdDirent->d_name);
            char buff[1024];
            ssize_t len = readlink(linkPath.c_str(), buff, 1024);
            if(len!=-1){
                buff[len] = '\0';
                if(len > 6){
                    string type = string(buff).substr(0, 6);
                    if(type == "socket"){
                        string inode = string(buff+8);
                        inode.erase(inode.length()-1);
                        //printf ("\t[%s] -> [%s]\n", fdDirent->d_name, buff);
                        socketfd->insert(inode, atoi(pDirent->d_name), cmdline);
                    }
                }
            }
        }
    }
    //socketfd->traverse(socketfd->root);
}

SocketInode* Processes::search(char* inode){
    return socketfd->search(inode);
}
