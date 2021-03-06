#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <regex>

using namespace std;

#include "Processes.h"

#define TCP 1
#define TCP6 2
#define UDP 4
#define UDP6 8


class Record{
    public:
        Record(int type);
        void SetLocalAddr(char* localAddr);
        void SetRemoteAddr(char* remoteAddr);
        void SetInfo(SocketInode* inode);
        int protocolType;
        string localAddr;
        string remoteAddr;
        int pid;
        string cmdline;
        string AddrConvert(char* localAddr, int type);
};

class NetInfo{
    public:
        const string CHROOT;
        NetInfo();
        NetInfo(int);
        vector<Record*> records;
        Processes *processes;
        void show();
        void show(vector<string> filter);
    private:
        int type;
        void GetRecord(int type);
};
