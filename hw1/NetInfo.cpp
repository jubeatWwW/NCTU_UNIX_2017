#include "NetInfo.h"

NetInfo::NetInfo() : CHROOT("/proc/net/"){
    processes = new Processes();
    type = 15;

    string* type = new string[4] {"tcp", "tcp6", "udp", "udp6"};

    for(int i=0; i<4; i++){
        string path = CHROOT + type[i];
        cout << "type: " << path << endl;
        FILE* fptr = fopen(path.c_str(), "r");
        char line[1024];
        fgets(line, 1024, fptr);
        while(fgets(line, 1024, fptr) != NULL){
            //cout << line << endl;
            char token[64];
            int ptr = 0, pos = 0, cnt = 0;
            Record* record = new Record();

            while(1 == sscanf(line + ptr, "%64s%n", token, &pos)){
                ptr += pos;
                switch(cnt){
                    case 1:
                        record->SetLocalAddr(strdup(token));                        
                        break;
                    case 2:
                        record->SetRemoteAddr(strdup(token));                        
                        break;
                    case 9:
                        record->SetInfo(processes->search(token));
                        break;
                }
                cnt++;
                //cout << cnt << endl;
            }
        }
        fclose(fptr);
    }
}

NetInfo::NetInfo(int type){
}

Record::Record(){
}

void Record::SetLocalAddr(char* localAddr){

}

void Record::SetRemoteAddr(char* remoteAddr){
}

void Record::SetInfo(SocketInode* inode){
    if(-1 != inode->pid)
        cout << inode->pid << " " << inode->cmdline << endl;
}
