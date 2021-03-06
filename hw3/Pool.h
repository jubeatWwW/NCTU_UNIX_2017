#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <queue>
#include <sstream>
#include <iterator>
#include <iostream>
using namespace std;

#include "Task.h"
#include "ProcGrp.h"

class Pool{
    public:
        Pool(string cmdline);
        string cmdline;
        vector<string> cmd;
        queue<Task> tasks;
        void execute(pid_t& pid, string& name, pid_t& lastjob, unsigned& spcmd, int jshpgid);

    private:
        void _parseToVector(string cmdline);
};
