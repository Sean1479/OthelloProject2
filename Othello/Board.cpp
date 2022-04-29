#include "stdafx.h"
#include "Board.h"


int drow[8] = { 0,0,1,1,1,-1,-1,-1 };
int dcol[8] = { 1,-1,1,0,-1,1,0,-1 };

Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            chess_board[i][j] = -1;
    chess_board[3][3] = 0;
    chess_board[4][4] = 0;
    chess_board[3][4] = 1;
    chess_board[4][3] = 1;
    lastXO = 1;
    laststep = make_pair(-1, -1);
}

void Board::Init()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            chess_board[i][j] = -1;
    chess_board[3][3] = 0;
    chess_board[4][4] = 0;
    chess_board[3][4] = 1;
    chess_board[4][3] = 1;
    lastXO = 1;
    laststep = make_pair(-1, -1);
}


int Board::sumscore() {
    int mark[2] = { 0 };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (chess_board[i][j] >= 0) mark[chess_board[i][j]]++;
    // ?! could be improve
    if (mark[0] == mark[1]) return 0;
    else if (mark[0] > mark[1]) return mark[0];
    else if (mark[0] < mark[1]) return -1 * mark[1];
}

bool Board::search(position pos, int XO, int d) {
    int row = pos.first + drow[d];
    int col = pos.second + dcol[d];
    if (chess_board[row][col] == XO) return false; // no same chess under this line

    while (0 <= row && row <= 7 && 0 <= col && col <= 7) {
        if (chess_board[row][col] == -1) return false;
        else if (chess_board[row][col] == XO) return true; //find same chess after different chess
        else {
            row += drow[d];
            col += dcol[d];
        }
    }
    return false; // after all search didnot find any then false
}

vector<position> Board::GetValidPosition(int nowXO, position lstep) {
    vector<position> result;
    position pos;
    //?
    if (nowXO == -1) nowXO = !(bool)lastXO;
    if (lstep == make_pair(-2, -2)) lstep = laststep; //maybe useless

#pragma omp parallel for // OpenMP
    for (int n = 0; n < 64; n++) {
        int i = n / 8;
        int j = n % 8;
        if (chess_board[i][j] == -1) {
            pos = make_pair(i, j);
            for (int d = 0; d < 8; d++) {//direct
                if (search(pos, nowXO, d)) {
                    result.push_back(pos);
                    break;
                }
            }
        }
    }
    if (result.size() == 0) result.push_back(make_pair(-1, -1));
    return result;
}

bool Board::isend() {
    if (GetValidPosition(1, make_pair(-1, -1))[0].first == -1 && GetValidPosition(0, make_pair(-1, -1))[0].first == -1) return true;
    return false;
}

void Board::flip(position pos, int XO, int d) {
    int row = pos.first + drow[d];
    int col = pos.second + dcol[d];
    int oppositeXO = !(bool)XO;
    while (0 <= row && row <= 7 && 0 <= col && col <= 7 && chess_board[row][col] == oppositeXO) {
        chess_board[row][col] = XO;
        row += drow[d];
        col += dcol[d];
    }
}

void Board::onemove(position pos) {
    lastXO = !lastXO; // chess turn
    if (pos.first != -1) { // if legal
        chess_board[pos.first][pos.second] = lastXO;
        for (int d = 0; d < 8; d++) {
            if (search(pos, lastXO, d)) {
                flip(pos, lastXO, d);
            }
        }
    }
    laststep = pos; //change laststep value
}

position Board::random_move() {
    vector<pair<int, int>> validpos;
    int n;
    validpos = GetValidPosition(-1, make_pair(-2, -2));
    n = rand() % validpos.size();
    onemove(validpos[n]);
    return laststep;
}

position Board::human_move(int px, int py) {
    vector<pair<int, int>> validpos;
    position playerchoose;
    
    validpos = GetValidPosition(-1, make_pair(-2, -2));
    if (validpos[0].first == -1) {
        AfxMessageBox("NO VALID POSITION CAN PUT");
        onemove(validpos[0]);
        return validpos[0];
    }
    else {
            playerchoose = make_pair(py, px);
            for (auto vpos : validpos) {
                if (vpos == playerchoose) {
                    onemove(playerchoose);
                    return playerchoose;
                    break;
                }
            }
    }
    return make_pair(-9999, -9999);
}

int Board::simulate() {
    int mark;
    int sXO = lastXO;
    int sBoard[8][8];
    position sstep = laststep;
    memcpy(sBoard, chess_board, sizeof(chess_board)); // store temporarily
    while (!isend()) {
        random_move();
    }
    mark = sumscore();
    memcpy(chess_board, sBoard, sizeof(chess_board)); // back
    laststep = sstep;
    lastXO = sXO;
    return mark;
}

void Board::graph_board(CButton buttons[10][10]) {
    int display[8][8];
    vector<position> vpos = GetValidPosition(-1, make_pair(-2, -2));
    memcpy(display, chess_board, sizeof(chess_board));
    if (laststep.first != -1) display[laststep.first][laststep.second] = 2;
    for (auto pos : vpos) display[pos.first][pos.second] = 3;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch (display[i][j]) {
            case 0: buttons[j + 1][i + 1].SetWindowText("X"); break;
            case 1: buttons[j + 1][i + 1].SetWindowText("O"); break;
            case 2: {
                if (lastXO == 0) buttons[j + 1][i + 1].SetWindowText("X!");
                else buttons[j + 1][i + 1].SetWindowText("O!");
                break; }
            case 3: buttons[j + 1][i + 1].SetWindowText("+"); break;
            case -1: buttons[j + 1][i + 1].SetWindowText(""); break;
            }
        }
    }
}

CString Board::print_score() {
    int mark[2] = { 0 };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (chess_board[i][j] != -1) mark[chess_board[i][j]]++;

    CString res;
    res.Format("X=%d : O=%d", mark[0], mark[1]);
    if (mark[0] > mark[1]){
        whoiswin = 0;
    }
    else if(mark[0] < mark[1]) {
        whoiswin = 1;
    }
    else if (mark[0] == mark[1]) {
        whoiswin = 2;
    }
    else {
        whoiswin = -1;
    }
    return res;
}