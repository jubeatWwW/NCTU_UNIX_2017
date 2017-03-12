#include <vector>

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
        
        void insert(unsigned int, unsigned int);
};
