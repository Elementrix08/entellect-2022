#include <iostream>
#include <queue>

using namespace std;

class Node {
  public:
    int score;
    Node(int val) { this->score = val; }
    bool operator<(const Node *&other) { return score < other->score; }
};

class NodeCompare {
  public:
    bool operator()(Node *&node1, Node *&node2) {
        return node1->score < node2->score;
    }
};

int main() {
    priority_queue<Node *, vector<Node *>, NodeCompare> queue;
    queue.push(new Node(5));
    queue.push(new Node(1));
    queue.push(new Node(3));
    queue.push(new Node(8));
    queue.push(new Node(9));

    while (queue.size()) {
        Node *top = queue.top();
        queue.pop();
        printf("%d ", top->score);
    }

    cout << "\n";
    return 0;
}