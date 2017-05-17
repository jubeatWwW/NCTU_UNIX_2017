#include "ProcGrp.h"

ProcGrps::ProcGrps(){
}

void ProcGrps::AddGrp(ProcGrp pg){
    this->groups.push_back(pg);
}

void ProcGrps::PopGrp(){
    if(this->groups.empty())
        return;
    
    ProcGrp pg = this->groups.back();
    pid_t lastpid = pg.lastjob;
    pid_t fgpid = pg.lead;
    int status;
    
    if(kill(lastpid, SIGCONT) == -1){
        perror("kill -CONT");
    }
    
    tcsetpgrp(0, fgpid);
    waitpid(lastpid, &status, WUNTRACED);
    
    if(WIFSTOPPED(status)){
    } else if(WIFEXITED(status)){
        this->groups.pop_back();
    } else {
    }
    tcsetpgrp(0, getpgrp());
}

void ProcGrps::BgGrp(){
    if(this->groups.empty())
        return;
    
    ProcGrp pg = this->groups.back();
    pid_t lastpid = pg.lastjob;
    if(kill(lastpid, SIGCONT) == -1){
        perror("kill -CONT");
    }
    
}

void ProcGrps::list(){
    int i=1;
    for(ProcGrp g : this->groups){
        printf("[%d] suspended: %s\n", i++, g.name.c_str());
    }
}


ProcGrp::ProcGrp(pid_t pid, string name, pid_t lastjob){
    this->lead = pid;
    this->name = name;
    this->lastjob = lastjob;
}
