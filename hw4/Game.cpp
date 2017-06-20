#include "Game.h"

void draw_blank(){
    draw_message("                                                    ", 0);
}

Game::Game(int sockfd, int role){
    this->sockfd = sockfd;
    this->curPlayer = PLAYER1;
    this->role = role;
    this->gameover = false;

	initscr();			// start curses mode 
	getmaxyx(stdscr, height, width);// get screen size

	cbreak();			// disable buffering
					// - use raw() to disable Ctrl-Z and Ctrl-C as well,
	halfdelay(1);			// non-blocking getch after n * 1/10 seconds
	noecho();			// disable echo
	keypad(stdscr, TRUE);		// enable function keys and arrow keys
	curs_set(0);			// hide the cursor

	init_colors();
}

bool Game::controller(){
    
    gameover = false;
    curPlayer = PLAYER1;
	clear();
	cx = cy = 3;
	init_board();
	draw_board();
	draw_cursor(cx, cy, 1);
	draw_score();
	refresh();

	attron(A_BOLD);
	move(height-1, 0);	printw("Arrow keys: move; Space: drop piece; R: reset; Q: quit");
	attroff(A_BOLD);

	while(true) {			// main loop
		int ch = getch();
		int moved = 0;

		switch(ch) {
		case ' ':
        {
            if(gameover){
                draw_blank();
                draw_message("Game is over, press R to restart or Q to leave", 1);
                refresh();
            } else {
                if(curPlayer != role){
                    draw_blank();
                    draw_message("It's not your turn", 1);
                    refresh();
                    continue;
                }
                
                if(DropPiece(cx, cy)){
                    string str = "p"+to_string(cx)+to_string(cy);
                    write(sockfd, str.c_str(), str.length());
                }
            }

        }
            break;
		case 'q':
		case 'Q':
            write(sockfd, "q", 2);
			return false;
            break;
		case 'r':
		case 'R':
            write(sockfd, "r", 2);
            return true;
            break;
		case 'k':
		case KEY_UP:
			draw_cursor(cx, cy, 0);
			cy = (cy-1+BOARDSZ) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'j':
		case KEY_DOWN:
			draw_cursor(cx, cy, 0);
			cy = (cy+1) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'h':
		case KEY_LEFT:
			draw_cursor(cx, cy, 0);
			cx = (cx-1+BOARDSZ) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'l':
		case KEY_RIGHT:
			draw_cursor(cx, cy, 0);
			cx = (cx+1) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		}

		if(moved) {
			refresh();
			moved = 0;
		}

		napms(1);		// sleep for 1ms
	}

	//endwin();			// end curses mode
    return false;
}

bool Game::DropPiece(int x, int y){
    
    if(0 != board[y][x]){
        draw_blank();
        draw_message("You can't drop pieces here", 1);
        refresh();
        return false;
    }
    
    
    int cnt = _DropPiece(x, y);
    
    if(cnt > 0){
        board[y][x] = curPlayer;
        
        draw_blank();
        draw_message(to_string(cnt).c_str(), 1);
        refresh();

        draw_board();
        draw_cursor(cx, cy, 1);
        pair<int, int>p = draw_score();
        if(p.first+p.second == 64){
            string winner = (p.first > p.second)? "PLAYER1 wins" :
                            (p.first < p.second)? "PLAYER2 wins" :
                            "DRAW!!";
            draw_blank();
            draw_message(winner.c_str(), 1);
            refresh();
            gameover = true;
        } else if(0 == p.first){
            draw_blank();
            draw_message("PLAYER2 wins", 1);
            refresh();
            gameover = true;
        } else if(0 == p.second){
            draw_blank();
            draw_message("PLAYER1 wins", 1);
            refresh();
            gameover = true;
        } else {
            if(CheckPiece(curPlayer))
                next();
            else{
                if(CheckPiece(curPlayer*(-1))){
                    string winner = (p.first > p.second)? "PLAYER1 wins" :
                                    (p.first < p.second)? "PLAYER2 wins" :
                                    "DRAW!!";
                    draw_blank();
                    draw_message(winner.c_str(), 1);
                    refresh();
                    gameover = true;
                } else {
                    draw_blank();
                    draw_message("No place to drop", 1);
                    refresh();
                }
            }
        }
        return true;
    } else {
        draw_blank();
        draw_message("You can't drop pieces here", 1);
        refresh();
        return false;
    }
}

int Game::_DropPiece(int x, int y){
    int mx[3] = {0, -1, 1};
    int my[3] = {0, -1, 1}; 
    int cnt = 0;

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(0==i && 0==j)
                continue;
            
            int nx = x+mx[i]; 
            int ny = y+my[j];
            if(nx < 0 || ny < 0 || nx > 7 || ny > 7)
                continue;
            
            bool flag = false;
            while(board[ny][nx] == curPlayer*(-1)){
                int nnx = nx+mx[i]; 
                int nny = ny+my[j];
                if(nnx < 0 || nny < 0 || nnx > 7 || nny > 7)
                    break;

                if(board[nny][nnx] == curPlayer){
                    flag = true;
                    break;
                }

                nx += mx[i];
                ny += my[j];
            }

            if(flag){
                cnt ++;
                nx = x+mx[i]; 
                ny = y+my[j];
                while(board[ny][nx] != curPlayer){
                    board[ny][nx] = curPlayer;
                    nx += mx[i];
                    ny += my[j];
                }
            }
        }
    }
    return cnt;
}

bool Game::CheckPiece(int player){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(_CheckPiece(i, j, player))
                return true;
        }
    }

    return false;
}

bool Game::_CheckPiece(int x, int y, int player){
    int mx[3] = {0, -1, 1};
    int my[3] = {0, -1, 1}; 

    if(0 != board[y][x])
        return false;
    
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(0==i && 0==j)
                continue;
            
            int nx = x+mx[i]; 
            int ny = y+my[j];
            if(nx < 0 || ny < 0 || nx > 7 || ny > 7)
                continue;
            
            while(board[ny][nx] == curPlayer){
                int nnx = nx+mx[i]; 
                int nny = ny+my[j];
                if(nnx < 0 || nny < 0 || nnx > 7 || nny > 7)
                    break;

                if(board[nny][nnx] == curPlayer*(-1)){
                    return true;
                }

                nx += mx[i];
                ny += my[j];
            }
        }
    }
    
    return false;
}

void Game::next(){
    curPlayer = (PLAYER1 == curPlayer)?PLAYER2:PLAYER1;
    string player = (PLAYER1 == curPlayer)?"PLAYER1":"PLAYER2"; 
    string str = "Now Playing: " + player;
    draw_blank();
    draw_message(str.c_str(), 0);
    refresh();
}
