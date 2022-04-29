#include "stdafx.h"
#include "Tree.h"


Tree::Tree(int nchess, Board board) {
    root = new Node(make_pair(-1, -1), 1, NULL, board.GetValidPosition(-1, make_pair(-2, -2)));
    subroot = root;
    tail = root;
    nowchess = nchess;
}

void Tree::Init(int nchess, Board board) {
    root = new Node(make_pair(-1, -1), 1, NULL, board.GetValidPosition(-1, make_pair(-2, -2)));
    subroot = root;
    tail = root;
    nowchess = nchess;
}

Node* Tree::expand(Board board) {
    Node* newNode;
    vector<position> possiblepos;
    bool suitable;
    for (auto vpos : tail->valid_postion) {
        suitable = false;
        for (auto cpos : tail->child) {
            if (vpos == cpos->pos) {
                suitable = true;
                break;
            }
        }
        if (!suitable) possiblepos.push_back(vpos);
    }
    int rp = rand() % possiblepos.size();
    board.onemove(possiblepos[rp]);
    newNode = new Node(possiblepos[rp], !(bool)tail->XO, tail, board.GetValidPosition(-1, make_pair(-2, -2)));
    tail->child.push_back(newNode);
    return newNode;
}

void Tree::nextnode(position nextpos, Board board) {
    for (auto child : subroot->child) {
        if (child->pos == nextpos) {
            subroot = child;
            return;
        }
    }
    // if no child suitable
    board.onemove(nextpos);
    Node* newNode = new Node(nextpos, !(bool)subroot->XO, subroot, board.GetValidPosition(-1, make_pair(-2, -2)));
    subroot = newNode;
}

void Tree::nextnodeclear(position nextpos, Board board) {
    for (auto child : subroot->child) {
        if (child->pos == nextpos) {
            subroot = child;
            subroot->score = 0;
            subroot->total = 0;
            subroot->child.clear();
            return;
        }
    }
    // if no child suitable
    board.onemove(nextpos);
    Node* newNode = new Node(nextpos, !(bool)subroot->XO, subroot, board.GetValidPosition(-1, make_pair(-2, -2)));
    subroot = newNode;
}

Node* Tree::best_child(Node* tarroot = NULL, double R = 1.41) {
    double maxresult = -99999999;
    double UCB;
    Node* bestnode = NULL;
    if (tarroot == NULL) tarroot = subroot;
    for (auto cpos : tarroot->child) {
        UCB = 1.0 * cpos->score / cpos->total + R * sqrt(log(tarroot->total) / cpos->total);
        if (UCB > maxresult) {
            maxresult = UCB;
            bestnode = cpos;
        }
    }
    return bestnode;
}

Board Tree::gettail(Board board) {
    tail = subroot;
    while (!board.isend()) {
        int vpos = tail->valid_postion.size();
        int cpos = tail->child.size();
        if (vpos != cpos) {
            tail = expand(board);
            board.onemove(tail->pos);
            break;
        }
        else {
            tail = best_child(tail);
            board.onemove(tail->pos);
        }
    }
    return board;
}

void Tree::backup(int result) {
    int who_win = result > 0 ? Xwin : Owin;
    result = abs(result);
    while (tail != subroot) {
        tail->total += 1;
        if (!(tail->XO ^ who_win)) tail->score += 1;
        tail = tail->parent;
    }
    tail->total += 1;
}