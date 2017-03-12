#include <vector>
#include <iostream>
using namespace std;

class SocketInode{
    public:

        unsigned int count;
        char* cmdline;
        unsigned int pid;
        SocketInode* trie[10];
};

class Socketfd{
    public:
        Socketfd();
        SocketInode* root;
        
        void insert(string socketfd, unsigned int pid);
};
