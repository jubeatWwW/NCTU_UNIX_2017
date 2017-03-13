#include <stdio.h>
#include <stdlib.h>
#include "NetInfo.h"
using namespace std;

int main(int argc, char *argv[]){
    
    NetInfo *netinfo = new NetInfo();
    netinfo->show();
}
