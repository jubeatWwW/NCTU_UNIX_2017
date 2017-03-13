#include "Socketfd.h"

Socketfd::Socketfd(){
    root = new SocketInode();
    inode = "";
}

void Socketfd::insert(string socketfd, int pid, char* cmdline){
    cout << "insert" << endl;
    SocketInode* current = root;
    for(unsigned i=0; i< socketfd.length(); i++){
        unsigned num = socketfd[i] - '0';
        if(NULL == current->trie[num]){
            current->trie[num] = new SocketInode();
        }
        current = current->trie[num];
    }

    current->count++;
    current->pid = pid;
    current->cmdline = cmdline;
}

void Socketfd::traverse(SocketInode* current){
    if(NULL == current){
        return;
    }

    for(int i=0; i<10; i++){
        if(NULL != current->trie[i]){
            inode += i+'0';
            if(-1 != current->trie[i]->pid){
                cout << "inode: " << inode << endl;
                cout <<"pid: " << current->trie[i]->pid << endl;
                cout << "cmd: " << current->trie[i]->cmdline << endl;
            }
            
            traverse(current->trie[i]);
            inode.erase(inode.length()-1);

        }
    }
}

SocketInode* Socketfd::search(char* inode){
    //cout << "search for " << inode << endl;
    SocketInode* current = root;
    while('\0' != *inode){
        unsigned int num = *inode - '0';
        if(NULL == current->trie[num]){
            cout << "cannot found inode" << endl;
            break;
        }

        current = current->trie[num];
        inode++;
    }

    return current;
}

SocketInode::SocketInode(){
    count = 0;
    pid = -1;
}
