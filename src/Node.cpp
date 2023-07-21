#include "Node.h"

Node::Node() { score = 0; };

Node::Node(int score, int row, int col, int stepAllowance, Node *parent) {
    this->score = score;
    this->row = row;
    this->col = col;
    this->parent = parent;
    this->stepAllowance = stepAllowance;
}

bool Node::operator==(const Node &other) {
    return this->row == other.row && this->col == other.col;
}