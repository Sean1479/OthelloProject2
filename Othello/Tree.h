#pragma once
#include "Node.h"
#include "Board.h"

class Tree
{
    int nowchess;
    Node* root;
    Node* subroot, *tail;

public:
    Tree(int nchess, Board board);
    void Init(int nchess, Board board);
    Node* expand(Board board);
    void nextnode(position nextpos, Board board);
    void nextnodeclear(position nextpos, Board board);
    Node* best_child(Node* tarRoot, double cof);
    Board gettail(Board board);
    void backup(int result);
};

