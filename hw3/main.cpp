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

    char cmdline[1024];
    printf("jsh >> ");
    while(fgets(cmdline, 1024, stdin) != NULL){    
        Pool* pool = new Pool(cmdline);
        pool->execute();
        printf("%s", cmdline);
        printf("jsh >> ");
    }
    printf("WTF\n");
    return 0;
}
