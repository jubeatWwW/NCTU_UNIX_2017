#include <vector>
#include <iostream>
#include <string>
using namespace std;

class SocketInode{
    public:
        SocketInode();
        unsigned int count;
        char* cmdline;
        int pid;
        SocketInode* trie[10];
};

class Socketfd{
    public:
        Socketfd();
        SocketInode* root;
        
        void insert(string socketfd, int pid, char* cmdline);
        SocketInode* search(char* inode);
        void traverse(SocketInode* current);

    private:
        string inode;
};
