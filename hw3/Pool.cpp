#include "Pool.h"

Pool::Pool(string cmdline){
    this->cmdline = cmdline;
    this->_parseToVector(cmdline);
    
    for(int i=0; i<this->cmd.size(); i++){
        Task task = (1 == this->cmd.size())? Task(string(cmd[i])) : 
                    (0 == i)? Task(string(cmd[i]), PIPEOUT) :
                    (this->cmd.size() - 1 == i)? Task(string(cmd[i]), PIPEIN) :
                    Task(string(cmd[i]), PIPEIN|PIPEOUT);
        
        if(task.error){
            perror("syntax error");
            break;
        }
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

void Pool::execute(pid_t& grppid, string& grpname, pid_t& lastjob, unsigned& spcmd, int jshpgid){
    
    int tasksNum = tasks.size();
    int pipes[tasksNum][2];
    pid_t pids[tasksNum];

    bool isfirst = true;

    while(!tasks.empty()){
        Task cur = tasks.front();
        int currentTaskId = tasksNum - tasks.size();
        
        if(cur.cmdType > 0){
            spcmd = cur.cmdType;
            return;
        }

        spcmd = 0;

        if(pipe(pipes[currentTaskId]) < 0){
            perror("pipe err\n");
        }
        
        int status;
        pids[currentTaskId] = fork();
        if(pids[currentTaskId] < 0){
            perror("fork err");
        } else if(0 == pids[currentTaskId]){
            
            if(PIPEIN & cur.pipedir){
                if(dup2(pipes[currentTaskId - 1][PIPERD], STDIN_FILENO) < 0)
                    perror("dup2 err");
                close(pipes[currentTaskId - 1][PIPERD]);
                close(pipes[currentTaskId - 1][PIPEWT]);
            }

            if(PIPEOUT & cur.pipedir){
                if(dup2(pipes[currentTaskId][PIPEWT], STDOUT_FILENO) < 0)
                    perror("dup2 out err");
                close(pipes[currentTaskId][PIPERD]);
                close(pipes[currentTaskId][PIPEWT]);
            }

            if(FILEIN&cur.filedir){
                int fd = open(cur.inputName.c_str(), O_RDONLY, 0644); 
                if(dup2(fd, STDIN_FILENO) < 0){
                    perror("dup2 err");
                }
                close(fd);
            }
            if(FILEOUT&cur.filedir){
                int fd = open(cur.outputName.c_str(), O_RDWR|O_CREAT|O_TRUNC, 0644);
                if(dup2(fd, STDOUT_FILENO) < 0)
                    perror("dup2 err");
                close(fd);
            }

            if(execvp(cur.cmdArgv[0], cur.cmdArgv.data()) < 0){
                perror("exec err");
                _exit(1);
            }

        } else {
            setpgid(pids[currentTaskId], pids[0]);
            
            if(isfirst){
                isfirst = false;
                grppid = pids[0];
                grpname = cmdline;
                tcsetpgrp(0, grppid);
            }
            
            if(PIPEIN & cur.pipedir){
                close(pipes[currentTaskId - 1][PIPERD]);
                close(pipes[currentTaskId - 1][PIPEWT]);
            }
            
            tasks.pop();
        }
        
        lastjob = pids[tasksNum - 1];
        for(int i=0; i<tasksNum; i++){
            waitpid(pids[i], &status, WUNTRACED);
            
            if(WIFSTOPPED(status)){
                printf("sig stopped\n");
                spcmd = PSTOP;
            } else if(WIFEXITED(status)){
                printf("sig exited\n");
            } else {
                printf("sig unknown\n");
            }
        }
        tcsetpgrp(0, jshpgid);
    }
}
