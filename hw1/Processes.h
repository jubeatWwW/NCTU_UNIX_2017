#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <fstream>
#include "Socketfd.h"
using namespace std;


class Processes{
    public:
        const string PATH;
        Socketfd* socketfd;

        Processes();
    private:
        void BuildSocketTrie();
};
