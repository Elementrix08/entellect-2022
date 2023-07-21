#pragma once

class Node;

struct Node {
    int score;
    int stepAllowance;
    int row, col;
    Node *parent = nullptr;

    Node();

    Node(int score, int row, int col, int stepAllowance,
         Node *parent = nullptr);

    int distanceTo(const Node *other);

    bool operator==(const Node &other);
    bool operator!=(const Node &other);
};

class NodeCompare {
  public:
    bool operator()(Node *&node1, Node *&node2) {
        return node1->score < node2->score;
    }
};