#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

#include "Processes.h"

#define TCP 1
#define UDP 2
#define TCP6 4
#define UDP6 8

class Record{
    public:
        Record();
        void SetLocalAddr(char* localAddr);
        void SetRemoteAddr(char* remoteAddr);
        void SetInfo(SocketInode* inode);
        int type;
        string localAddr;
        string remoteAddr;
        int pid;
        string cmdline;
};

class NetInfo{
    public:
        const string CHROOT;
        NetInfo();
        NetInfo(int);
        vector<Record> records;
        Processes *processes;

    private:
        int type;   
};
