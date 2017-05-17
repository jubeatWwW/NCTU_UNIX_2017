#include "ProcGrp.h"

ProcGrps::ProcGrps(){
}

void ProcGrps::AddGrp(ProcGrp pg){
    this->groups.push_back(pg);
}

void ProcGrps::list(){
    for(ProcGrp g : this->groups){
        printf("pid: %d, name: %s\n", g.lead, g.name.c_str());
    }
}


ProcGrp::ProcGrp(pid_t pid, string name){
    this->lead = pid;
    this->name = name;
}
