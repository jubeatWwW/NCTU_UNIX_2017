#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#include "othello.h"

static int width;
static int height;
static int cx = 3;
static int cy = 3;

class Game{
    public:
        Game(int sockfd, int role);
        bool controller();
        bool DropPiece(int x, int y);
        bool CheckPiece();
        int sockfd;
        int curPlayer;
        int role;
        bool gameover;
    private:
        void next();
        int _DropPiece(int x, int y);
        bool _CheckPiece(int x, int y);
};
