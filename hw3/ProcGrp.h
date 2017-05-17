#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>
#include <queue>
#include <sstream>
#include <iterator>
#include <iostream>
using namespace std;

class ProcGrp{
    public:
        ProcGrp(pid_t pid, string name, pid_t lastjob);
        pid_t lead;
        pid_t lastjob;
        string name;
};

class ProcGrps{
    public:
        ProcGrps();
        vector<ProcGrp> groups;
        void AddGrp(ProcGrp pg);
        void PopGrp();
        void RmGrp(pid_t pid);
        void list();
};

