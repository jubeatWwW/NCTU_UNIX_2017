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
#define FILEIN 1
#define FILEOUT 2

#define EXIT 1
#define EXPORT 2
#define UNSET 3
#define JOBS 4
#define FG 5
#define BG 6
#define PSTOP 7

class Task{
    public:
        Task(string cmd);
        Task(string cmd, unsigned pipedir);
        Task(string cmd, unsigned pipedir, unsigned filedir);
        string cmd;
        vector<char*> cmdArgv;
        unsigned pipedir;
        unsigned filedir;
        string inputName;
        string outputName;
        bool error;
        unsigned cmdType;
    private:
        unsigned _builtInNo(string cmd);
        void _parseToArgv(string cmd);
};
