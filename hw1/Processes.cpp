#include "Processes.h"

Processes::Processes() : PATH("/proc"){
    DIR *pDir;
    dirent* pDirent;
    pDir = opendir(PATH.c_str());

    while((pDirent = readdir(pDir)) != NULL){
        printf ("[%s]\n", pDirent->d_name);

        char flag = *pDirent->d_name;
        if(flag >= '0' && flag <= '9'){
            string fdPath = PATH + "/" + string(pDirent->d_name) + "/fd";
            cout << fdPath << endl;
            dirent* fdDirent;
            DIR* fdDir = opendir(fdPath.c_str());

            while((fdDirent = readdir(fdDir)) != NULL){
                
                string linkPath = fdPath + "/" + string(fdDirent->d_name);
                char buff[256];
                ssize_t len = readlink(linkPath.c_str(), buff, 256);
                if(len!=-1){
                    buff[len] = '\0';
                    printf ("\t[%s] -> [%s]\n", fdDirent->d_name, buff);
                }
                
            }
        }
    }
    
}
