#include "Processes.h"

Processes::Processes() : PATH("/proc"){

    socketfd = new Socketfd();

    DIR *pDir;
    dirent* pDirent;
    pDir = opendir(PATH.c_str());

    while((pDirent = readdir(pDir)) != NULL){
        if(!isdigit(*pDirent->d_name))
            continue;

        printf ("[%s]\n", pDirent->d_name);

        string fdPath = PATH + "/" + string(pDirent->d_name) + "/fd";
        cout << fdPath << endl;
        dirent* fdDirent;
        DIR* fdDir = opendir(fdPath.c_str());

        while((fdDirent = readdir(fdDir)) != NULL){
            if(!isdigit(*fdDirent->d_name))
                continue;
            
            string linkPath = fdPath + "/" + string(fdDirent->d_name);
            char buff[256];
            ssize_t len = readlink(linkPath.c_str(), buff, 256);
            if(len!=-1){
                buff[len] = '\0';
                string type = string(buff).substr(0, 6);
                if(type == "socket"){
                    string inode = string(buff+8);
                    inode.erase(inode.length()-1);
                    cout << inode << endl;
                    printf ("\t[%s] -> [%s]\n", fdDirent->d_name, buff);
                    socketfd->insert(inode, atoi(pDirent->d_name));
                }
            }
            
        }

    }
    socketfd->traverse(socketfd->root);
    
}
