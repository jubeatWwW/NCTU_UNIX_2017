#include "Pool.h"

Pool::Pool(string cmdline){
    this->cmdline = cmdline;
    this->_parseToVector(cmdline);
    
    for(int i=0; i<this->cmd.size(); i++){
        Task task = Task(string(cmd[i]));
        this->tasks.push(task);
    }

}

void Pool::_parseToVector(string cmdline){
    
    bool dblQuoteFlag = false, sglQuoteFlag = false;
    for(int i=0,j=0; j<cmdline.length(); j++){
        if(j > 0 && '\\' != cmdline[j-1]){
            if(!sglQuoteFlag && '\"' == cmdline[j])
                dblQuoteFlag ^= true;
            if(!dblQuoteFlag && '\'' == cmdline[j])
                sglQuoteFlag ^= true;
        }
        
        if(j == cmdline.length()-1 || !(dblQuoteFlag|sglQuoteFlag) && '|' == cmdline[j+1]){
            this->cmd.push_back(cmdline.substr(i, (j++)-i+1));
            while(' ' == cmdline[j+1]) j++;
            i = j+1;
        }
    }

    /*for(int i=0; i< this->cmd.size(); i++){
        cout << this->cmd[i] << endl;
    }*/
}


void Pool::execute(){
    int pipefd1[2];
    int pipefd2[2];
    bool isfirst = true;
    bool checkbit = true;
    char buffer[1024];
    int len = 0;

    while(!tasks.empty()){
        if(pipe(pipefd2) < 0){
            perror("pipe fd2 err\n");
        }
        
        pid_t pid;
        int status;

        if((pid = fork()) < 0){
            perror("fork err");
        } else if(0 == pid){
            if(isfirst && 1 == tasks.size()){
                if(execvp(tasks.front().cmdArgv[0], tasks.front().cmdArgv.data()) < 0){
                    perror("exec err");
                    _exit(1);
                }
            } else {

                close(pipefd2[PIPERD]);

                if(!isfirst){
                    if(dup2(pipefd1[PIPERD], STDIN_FILENO) < 0)
                        perror("dup2 err");
                    close(pipefd1[PIPERD]);
                }

                if(1 < tasks.size()){
                    if(dup2(pipefd2[PIPEWT], STDOUT_FILENO) < 0)
                        perror("dup2 err");
                    close(pipefd2[PIPEWT]);
                }
                
                if(execvp(tasks.front().cmdArgv[0], tasks.front().cmdArgv.data()) < 0){
                    perror("exec err");
                    _exit(1);
                }
            }
        } else {
            waitpid(pid, &status, WUNTRACED|WCONTINUED);
            
            close(pipefd2[PIPEWT]);
            close(pipefd1[PIPERD]);

            pipefd1[PIPEWT] = pipefd2[PIPEWT];
            pipefd1[PIPERD] = pipefd2[PIPERD];
            
            if(WIFSTOPPED(status)){
                printf("sig stopped\n");
            } else if(WIFEXITED(status)){
                printf("sig exited\n");
            } else {
                printf("sig unknown\n");
            }
            
            isfirst = false;
            tasks.pop();
        }
    }
}
