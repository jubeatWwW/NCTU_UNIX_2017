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
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
    ProcGrps* procGrps = new ProcGrps();

    string cmdline;
    printf("jsh >> ");
    while(getline(cin, cmdline)){    
        
        
        if("" == cmdline){
            printf("jsh >> ");
            continue;
        }
        
        unsigned spcmd;
        int pid;
        string name;
        Pool* pool = new Pool(cmdline);
        pool->execute(pid, name, spcmd);

        if(EXIT == spcmd) break;
        
        if(PSTOP == spcmd){
            procGrps->AddGrp(ProcGrp(pid, name));
            procGrps->list();
        }

        if(FG == spcmd){
        }

        printf("jsh >> ");
    }
    printf("BYE\n");
    return 0;
}
