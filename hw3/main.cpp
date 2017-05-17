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

int main(){
 
    signal(SIGTSTP, SIG_IGN);   
    //signal(SIGINT, SIG_IGN);
    //signal(SIGQUIT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    ProcGrps* procGrps = new ProcGrps();
    int jshpgid = getpgid(jshpgid = getpid());

    string cmdline;
    printf("jsh >> ");
    while(getline(cin, cmdline)){    
        
        
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
            case JOBS:
                procGrps->list();

        }

        printf("jsh >> ");
    }
    printf("BYE\n");
    return 0;
}
