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

    printf("jsh >> ");
    int i[2]= {0, 0};
    for(string cmdline; getline(cin, cmdline);){
        
        Pool* pool = new Pool(cmdline);
        pool->execute();
        printf("jsh >> ");
    }
    
    return 0;
}
