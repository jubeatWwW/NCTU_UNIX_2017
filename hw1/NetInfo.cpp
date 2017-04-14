#include "NetInfo.h"

const int PROTOCOL[4] = {1, 2, 4, 8};

NetInfo::NetInfo() : CHROOT("/proc/net/"){
    processes = new Processes();
    this->type = 15;
    GetRecord(type);
}

NetInfo::NetInfo(int type): CHROOT("/proc/net/"){
    processes = new Processes();
    this->type = type;
    GetRecord(type);
}

void NetInfo::GetRecord(int type){
    string* typeName = new string[4] {"tcp", "tcp6", "udp", "udp6"};
    
    int begin = 0, end = 3;

    if(~type&(TCP|TCP6))  begin = 2;
    if(~type&(UDP|UDP6))  end = 1;
    while(begin <= end){
        string path = CHROOT + typeName[begin];
        FILE* fptr = fopen(path.c_str(), "r");
        char line[1024];
        if(fptr)
            fgets(line, 1024, fptr);
        while(fgets(line, 1024, fptr) != NULL){
            //cout << line << endl;
            char token[65];
            int ptr = 0, pos = 0, cnt = 0;

            Record* record = new Record(PROTOCOL[begin]);

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
        begin++;
    }
    
}

void NetInfo::show(){
    string* typeName = new string[4] {"tcp", "tcp6", "udp", "udp6"};
    bool tcpflag = false, udpflag = false;

    for(Record* record : records){
        if(!tcpflag && (record->protocolType&(TCP|TCP6))){
            cout << endl << "List of TCP connections:" << endl;
            cout << left << setw(8) << "Proto";
            cout << left << setw(32) << "Local Address" << left << setw(32) << "Foreign Address";
            cout << left << "PID/Program name and arguments" << right << endl;
            tcpflag = true;
        }
        if(!udpflag && (record->protocolType&(UDP|UDP6))){
            cout << endl << "List of UDP connections:" << endl;
            cout << left << setw(8) << "Proto";
            cout << left << setw(32) << "Local Address" << left << setw(32) << "Foreign Address";
            cout << left << "PID/Program name and arguments" << right << endl;
            udpflag =true;
        }
        int typeStrNum = log(record->protocolType)/log(2);

        stringstream buffer;

        buffer << left << setw(8) << typeName[typeStrNum];

        buffer << left << setw(32) << record->localAddr << left << setw(32) << record->remoteAddr;
        if(-1 == record->pid)
            buffer << left << "-" << endl;
        else
            buffer << left <<record->pid << "/" << record->cmdline << right << endl;
        cout << buffer.str();   
    }
}

void NetInfo::show(vector<string> filter){
    string* typeName = new string[4] {"tcp", "tcp6", "udp", "udp6"};
    bool tcpflag = false, udpflag = false;
    
    for(Record* record : records){
        
        int typeStrNum = log(record->protocolType)/log(2);
        
        stringstream buffer;

        buffer << left << setw(8) << typeName[typeStrNum];

        buffer << left << setw(32) << record->localAddr << left << setw(32) << record->remoteAddr;
        if(-1 == record->pid)
            buffer << left << "-" << endl;
        else
            buffer << left <<record->pid << "/" << record->cmdline << right << endl;
        
        bool match = true;
        for(unsigned i=0; i<filter.size(); i++){
            regex re(filter[i]);
            if(regex_search(buffer.str().c_str(), re)){
                //cout << "Match" << endl;
            } else {
                match = false;
            }
        }

        if(!match) continue;

        if(!tcpflag && (record->protocolType&(TCP|TCP6))){
            cout << endl << "List of TCP connections:" << endl;
            cout << left << setw(8) << "Proto";
            cout << left << setw(32) << "Local Address" << left << setw(32) << "Foreign Address";
            cout << left << "PID/Program name and arguments" << right << endl;
            tcpflag = true;
        }
        if(!udpflag && (record->protocolType&(UDP|UDP6))){
            cout << endl << "List of UDP connections:" << endl;
            cout << left << setw(8) << "Proto";
            cout << left << setw(32) << "Local Address" << left << setw(32) << "Foreign Address";
            cout << left << "PID/Program name and arguments" << right << endl;
            udpflag =true;
        }
        cout << buffer.str();   
    }
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
            string subip = "";
            for(int j=0; j<4; j++){
                char part[3] = {*addr, *(addr+1), '\0'};
                subip = string(part) + subip;
                addr+=2;
            }
            ip += subip;
        }
        
        string shortip = "";
        bool isFirst = true;
        int zCnt = 0;
        for(int i=0; i<8; i++){
            string subip = "";
            bool allz = true;
            for(int j=0; j<4; j++){
                if('0' != ip[i*4 + j]){
                    subip += ip[i*4 + j];
                    allz = false;
                }
            }

            if(allz){
                zCnt++;
                if(!isFirst) 
                    shortip += "0:";
            } else {
                if(isFirst && 2 <= zCnt){
                    shortip += "::" + subip + ":";
                    isFirst = false;
                } else if(isFirst && 1 == zCnt){
                    shortip += "0:" + subip + ":";
                } else {
                    shortip += subip + ":";
                }
                zCnt = 0;
            }
        }

        if(isFirst) 
            shortip = ":::";
        else if(zCnt > 0){
            while(zCnt--){
                shortip += "0:";
            }
        }
        
        addr++;
        long port = strtoul(addr, NULL, 16);
        shortip += 0==port ? "*" : to_string(port);
        //cout << ip << endl;
        return shortip;
        
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
