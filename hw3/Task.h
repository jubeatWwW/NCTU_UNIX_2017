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

#define PIPEOUT 1
#define PIPEIN 2
#define PIPERD 0
#define PIPEWT 1

class Task{
    public:
        Task(string cmd);
        string cmd;
        vector<char*> cmdArgv;
        void execute(unsigned stddir, int pipefdIn[2], int pipefdOut[2]);
    private:
        void _parseToArgv(string cmd);
};
