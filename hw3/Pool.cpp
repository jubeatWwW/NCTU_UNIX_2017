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

    while(!tasks.empty()){

        if(isfirst){
            if(1 == tasks.size())
                tasks.front().execute(0, pipefd1, pipefd2);
            else
                tasks.front().execute(PIPEOUT, pipefd1, pipefd2);
            isfirst = false;
        } else {
            int direct = (1 == tasks.size())? PIPEIN : PIPEIN|PIPEOUT;

            if(checkbit)
                tasks.front().execute(direct, pipefd1, pipefd2);
            else
                tasks.front().execute(direct, pipefd2, pipefd1);
        }
        
        checkbit ^= true;
        tasks.pop();
    }

    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);
}
