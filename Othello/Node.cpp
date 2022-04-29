#include "stdafx.h"
#include "Node.h"


Node::Node(position p, int c, Node* par, vector<position> posv) {
    pos = p;
    XO = c;
    parent = par;
    total = 0;
    score = 0;
    valid_postion = posv;
}
