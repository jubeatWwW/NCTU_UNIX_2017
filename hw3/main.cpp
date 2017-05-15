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

    string cmdline;
    printf("jsh >> ");
    while(getline(cin, cmdline)){    
        if("" == cmdline){
            printf("empty command\n");
            printf("jsh >> ");
            continue;
        }
        Pool* pool = new Pool(cmdline);
        pool->execute();
        printf("%s\n", cmdline.c_str());
        printf("jsh >> ");
    }
    printf("WTF\n");
    return 0;
}
