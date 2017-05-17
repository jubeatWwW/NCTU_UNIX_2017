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
void Pool::execute(pid_t& grppid, string& grpname, unsigned& spcmd){
    int pipefd1[2];
    int pipefd2[2];
    
    bool isfirst = true;

    while(!tasks.empty()){
        Task cur = tasks.front();
        
        if(cur.cmdType > 0){
            spcmd = cur.cmdType;
            break;
        }

        if(pipe(pipefd2) < 0){
            perror("pipe fd2 err\n");
        }
        
        pid_t pid;
        int status;

        if((pid = fork()) < 0){
            perror("fork err");
        } else if(0 == pid){

            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);   

            //printf("file: %s\n", cur.inputName.c_str());
            //printf("filedir: %d\n", cur.filedir);
            
            if((PIPEIN|PIPEOUT)&cur.pipedir)
                close(pipefd2[PIPERD]);
            
            if(FILEIN&cur.filedir){
                int fd = open(cur.inputName.c_str(), O_RDONLY, 0644); 
                if(dup2(fd, STDIN_FILENO) < 0){
                    perror("dup2 err");
                }
                close(fd);
            } else if(PIPEIN & cur.pipedir){
                if(dup2(pipefd1[PIPERD], STDIN_FILENO) < 0)
                    perror("dup2 err");
                close(pipefd1[PIPERD]);
            }

            if(FILEOUT&cur.filedir){
                int fd = open(cur.outputName.c_str(), O_RDWR|O_CREAT|O_TRUNC);
                if(dup2(fd, STDOUT_FILENO) < 0)
                    perror("dup2 err");
                close(fd);
            } else if(PIPEOUT & cur.pipedir){
                if(dup2(pipefd2[PIPEWT], STDOUT_FILENO) < 0)
                    perror("dup2 err");
                close(pipefd2[PIPEWT]);
            }
            if(execvp(cur.cmdArgv[0], cur.cmdArgv.data()) < 0){
                perror("exec err");
                _exit(1);
            }

        } else {
            if(isfirst){
                isfirst = false;
                grppid = pid;
                grpname = cmdline;
            }
            setpgid(pid, grppid);

            waitpid(pid, &status, WUNTRACED|WCONTINUED);
            

            close(pipefd2[PIPEWT]);
            close(pipefd1[PIPERD]);

            pipefd1[PIPEWT] = pipefd2[PIPEWT];
            pipefd1[PIPERD] = pipefd2[PIPERD];
            
            if(FILEOUT&cur.filedir){
                int fd = open(cur.outputName.c_str(), O_RDONLY, 0644); 
                pipefd1[PIPERD] = fd;
            }

            if(WIFSTOPPED(status)){
                printf("sig stopped\n");
                spcmd = PSTOP;
            } else if(WIFEXITED(status)){
                printf("sig exited\n");
            } else {
                printf("sig unknown\n");
            }
            
            tasks.pop();
        }
    }
}
