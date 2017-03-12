#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "Socketfd.h"
using namespace std;


class Processes{
    public:
        const string PATH;
        Socketfd socketfd;

        Processes();
};
