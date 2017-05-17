#include "Task.h"

Task::Task(string cmd){
    this->error = false;
    this->pipedir = 0;
    this->filedir = 0;
    this->cmd = cmd;
    this->_parseToArgv(cmd);
}

Task::Task(string cmd, unsigned pipedir){
    this->error = false;
    this->pipedir = pipedir;
    this->filedir = 0;
    this->cmd = cmd;
    this->_parseToArgv(cmd);
}

Task::Task(string cmd, unsigned pipedir, unsigned filedir){
    this->error = false;
    this->pipedir = pipedir;
    this->filedir = filedir;
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
    
    this->cmdType  = this->_builtInNo(arg[0]);

    for(int i=0; i<arg.size(); i++){
        char* str = (char*)malloc(1024 * sizeof(char));
        
        if("<" == arg[i]){
            this->filedir |= FILEIN;
            if(arg.size() > i+1){
                this->inputName = arg[i+1];
                i++;
                continue;
            } else {
                this->error = true;
                break;
            }
        }

        if(">" == arg[i]){
            this->filedir |= FILEOUT;
            if(arg.size() > i+1){
                this->outputName = arg[i+1];
                i++;
                continue;
            } else {
                this->error = true;
                break;
            }
        }

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

unsigned Task::_builtInNo(string cmd){
    if("exit" == cmd){
        return EXIT;
    } else if("export" == cmd){
        return EXPORT;
    } else if("unset" == cmd){
        return UNSET;
    } else if("jobs" == cmd){
        return JOBS;
    } else if("fg" == cmd){
        return FG;
    } else if("bg" == cmd){
        return BG;
    } else {
        return 0;
    }
}
