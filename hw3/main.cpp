#include <stdio.h>
#include <stdlib.h>
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

#include "Pool.h"

void shExport(const string& envstr){
    char *envdup = strdup(envstr.c_str());
    char *delim = strchr(envdup, '=');
    
    if(NULL == delim){
        printf("syntax error\n");
        return;
    }

    char *key = envdup;
    *delim = '\0';
    char *val = delim + 1;
    
    if(setenv(key, val, 1) < 0){
        perror("setenv");
    }

    free(envdup);
}

int main(){
    ProcGrps* procGrps = new ProcGrps();
    int jshpid = 0;
    int jshpgid = getpgid(jshpid = getpid());
 
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGINT, SIG_IGN);


    string cmdline;
    printf("jsh >> ");
    while(getline(cin, cmdline)){    
        
        while(' ' == cmdline[0]){
            cmdline.erase(cmdline.begin());
        }

        if("" == cmdline){
            printf("jsh >> ");
            continue;
        }
        
        unsigned spcmd;
        pid_t pid, lastjob;
        string name;
        Pool* pool = new Pool(cmdline);
        pool->execute(pid, name, lastjob, spcmd, jshpgid);

        if(EXIT == spcmd) break;

        switch(spcmd){
            case PSTOP:
                procGrps->AddGrp(ProcGrp(pid, name, lastjob));
                break;
            case FG:
                procGrps->PopGrp();
                break;
            case BG:
                procGrps->BgGrp();
                break;
            case JOBS:
                procGrps->list();
                break;
            case EXPORT:
                if(cmdline.length() < 7)
                    printf("export: syntax error\n");
                else
                    shExport(pool->tasks.front().cmdArgv[1]);
                break;
            case UNSET:
                if(cmdline.length() < 6)
                    printf("export: syntax error\n");
                else
                    unsetenv(pool->tasks.front().cmdArgv[1]);
                break;

        }

        printf("jsh >> ");
    }
    printf("BYE!!\n");
    return 0;
}
