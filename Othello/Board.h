#pragma once

class Board
{
    int chess_board[8][8];
    int lastXO;
    position laststep;

public:
    Board();
    void Init();
    int sumscore();
    bool isend();
    bool search(position p, int player, int d);
    void flip(position p, int player, int d);
    void onemove(position p);
    vector<position> GetValidPosition(int, position);
    position random_move();
    position human_move(int px, int py);
    int simulate();
    void graph_board(CButton buttons[10][10]);
    CString print_score();
    int whoiswin;
};