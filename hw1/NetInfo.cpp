#include "NetInfo.h"

const int PROTOCOL[4] = {1, 2, 4, 8};

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

            Record* record = new Record(PROTOCOL[i]);

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

            records.push_back(record);
        }
        fclose(fptr);
    }
}

void NetInfo::show(){
    for(Record* record : records){
        cout << record->localAddr << "\t";
        cout << record->remoteAddr << "\t";
        if(-1 == record->pid)
            cout << "-" << endl;
        else
            cout << record->pid << "/" << record->cmdline << endl;
    }
}

NetInfo::NetInfo(int type){
}

Record::Record(int type){
    protocolType = type;
}

void Record::SetLocalAddr(char* localAddr){
    this->localAddr = AddrConvert(localAddr, protocolType);
}

void Record::SetRemoteAddr(char* remoteAddr){
    this->remoteAddr = AddrConvert(remoteAddr, protocolType);
}

string Record::AddrConvert(char* addr, int type){
    string ip = "";
    if(type&(TCP|UDP)){
        for(int i=0; i<4; i++){
            char part[3] = {*addr, *(addr+1), '\0'};
            ip = to_string(strtoul(part, NULL, 16)) + "." + ip;
            addr+=2;
        }
        addr++;
        ip[ip.length()-1] = ':';
        long port = strtoul(addr, NULL, 16);
        ip += 0==port ? "*" : to_string(port);
        //cout << ip << endl;
        return ip;
    } else {
        for(int i=0; i<4; i++){
            bool flag = false;
            string subip = "";
            for(int j=0; j<4; j++){
                char part[3] = {*addr, *(addr+1), '\0'};
                if('0' != *addr || '0' != *(addr+1))
                    flag = true;
                subip = string(part) + subip;
                addr+=2;
            }
            if(flag)
                ip += subip+":";
            else
                ip += ":";
        }
        
        addr++;
        long port = strtoul(addr, NULL, 16);
        ip += 0==port ? "*" : to_string(port);
        //cout << ip << endl;
        return ip;
        
    }

}

void Record::SetInfo(SocketInode* inode){
    if(-1 != inode->pid){
        //cout << inode->pid << " " << inode->cmdline << endl;
        pid = inode->pid;
        cmdline = string(inode->cmdline);
    } else {
        pid = -1;
    }
}
