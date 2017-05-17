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
    for(size_t i=0; i<cmd.length(); i++){
        if('\\' == cmd[i])
            cmd.erase(i, 1);
    }
    istringstream iss(cmd);
    vector<string> args{istream_iterator<string>{iss}, 
                        istream_iterator<string>{}
                       };

    vector<string> arg;
    for(string s : args){
        if(string::npos != s.find("*") || string::npos != s.find("?")){
            glob_t glob_res;
            glob(s.c_str(), GLOB_TILDE, NULL, &glob_res);
            for(size_t i=0 ; i<glob_res.gl_pathc ; i++){
                arg.push_back(glob_res.gl_pathv[i]);
            }
            if(!glob_res.gl_pathv){
                arg.push_back(s);
            }
        } else {
            arg.push_back(s);
        }
    }

    vector<char*> argv;
    
    this->cmdType  = this->_builtInNo(arg[0]);

    for(size_t i=0; i<arg.size(); i++){
        char* str = strdup(arg[i].c_str());
        
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

        argv.push_back(str);
    }
    argv.push_back((char*)0);

    this->cmdArgv = argv;
}

unsigned Task::_builtInNo(string cmd){
    return ("exit" == cmd)?     EXIT :
           ("export" == cmd)?   EXPORT :
           ("unset" == cmd)?    UNSET :
           ("jobs" == cmd)?     JOBS :
           ("fg" == cmd)?       FG :
           ("bg" == cmd)?       BG : 0;
}
