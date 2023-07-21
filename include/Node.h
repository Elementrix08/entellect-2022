struct Node;

struct Node {
    int score;
    int stepAllowance;
    int row, col;
    Node *parent = nullptr;

    Node();

    Node(int score, int row, int col, int stepAllowance,
         Node *parent = nullptr);

    bool operator==(const Node &other);
};