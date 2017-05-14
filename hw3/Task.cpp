#include "Task.h"

Task::Task(string cmd){
    this->cmd = cmd;
    this->_parseToArgv(cmd);
}

void Task::_parseToArgv(string cmd){
    for(int i=0; i<cmd.length(); i++){
        if('\\' == cmd[i])
            cmd.erase(i, 1);
    }
    istringstream iss(cmd);
    vector<string> arg{istream_iterator<string>{iss}, 
                        istream_iterator<string>{}
                       };
    vector<char*> argv;

    for(int i=0; i<arg.size(); i++){
        char* str = (char*)malloc(1024 * sizeof(char));
        int j;
        for(j=0; j<arg[i].length(); j++){
            str[j] = arg[i][j];
        }
        str[j] = '\0';

        argv.push_back(str);
    }
    argv.push_back((char*)0);

    this->cmdArgv = argv;
}

void Task::execute(unsigned stddir, int pipefdIn[2], int pipefdOut[2]){
    /*for(char* s : argv){
        printf("argv: %s\n", s);
    }*/


    int status;
    pid_t pid;
    
    if(stddir&PIPEIN){
        close(pipefdIn[PIPEWT]);
        if(dup2(pipefdIn[PIPERD], STDIN_FILENO) < 0){
            printf("dup2 in failed\n");
        }
        close(pipefdIn[PIPERD]);
    }

    if((pid = fork()) < 0){
        printf("ERROR fork\n");
        exit(1);       
    } else if(0 == pid){
        
        if(stddir&PIPEOUT){
            close(pipefdOut[PIPERD]);
            if(dup2(pipefdOut[PIPEWT], STDOUT_FILENO) < 0){
                printf("dup2 out failed\n");
            }
        }       

        if(execvp(this->cmdArgv[0], this->cmdArgv.data()) < 0){
            printf("ERROR execvp\n");
            exit(1);
        }
    } else {
        waitpid(-1, &status, WUNTRACED|WCONTINUED);
    }
}

