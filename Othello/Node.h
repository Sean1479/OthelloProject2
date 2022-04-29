#pragma once
#include <cstdio>

#pragma once

class Node
{
public:
    position pos;
    int total, score;
    int XO;
    Node* parent;
    vector<Node*> child;
    vector<pair<int, int>> valid_postion;
    Node(position p, int c, Node* par, vector<position> v);
};

