#include "Node.h"
#include <cmath>

Node::Node() { score = 0; };

Node::Node(int score, int row, int col, int stepAllowance, Node *parent) {
    this->score = score;
    this->row = row;
    this->col = col;
    this->parent = parent;
    this->stepAllowance = stepAllowance;
}

int Node::distanceTo(const Node *other) {
    return abs(row - other->row) + abs(col - other->col);
}

bool Node::operator!=(const Node &other) {
    return this->row != other.row && this->col != other.col;
}

bool Node::operator==(const Node &other) {
    return this->row == other.row && this->col == other.col;
}