#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "NetInfo.h"
using namespace std;

const char* shortOpt = "ut";
const struct option longOpt[] = {
    {"tcp", 0, NULL, 't'},
    {"udp", 0, NULL, 'u'},
    {NULL, 0, NULL, 0}
};

int main(int argc, char **argv){
    int c;

    int type = TCP|TCP6|UDP|UDP6;

    while(-1 != (c = getopt_long(argc, argv, shortOpt, longOpt, NULL))){
        switch(c){
            case 't':
                type &= (TCP|TCP6);
                break;
            case 'u':
                type &= (UDP|UDP6);
                break;
        }
    }
    

    if(!type) type = 15;
    
    NetInfo *netinfo = new NetInfo(type);

    vector<string> filter;
    int filterNum = argc - optind;
    
    if(0 < filterNum){
        for(int i=0; i<filterNum; i++){
            filter.push_back(string(argv[optind++]));
        }

        netinfo->show(filter);
    } else {
        netinfo->show();
    }

    
    return 0;
}
