#include "Solver.h"
#include <iostream>
#include <queue>

using namespace std;

map<string, int> Solver::travelDifficulty = {
    {"S", 1}, {"I", 5}, {"TS", 10}, {"M", 15}};

Solver::Solver() {
    cin >> stepAllowance;

    cin >> numCoal;
    coalPoints = vector<Point>(numCoal);

    for (int i = 0; i < numCoal; ++i)
        cin >> coalPoints[i].x >> coalPoints[i].y;

    cin >> numFish;
    fishPoints = vector<Point>(numFish);

    for (int i = 0; i < numFish; ++i)
        cin >> fishPoints[i].x >> fishPoints[i].y;

    cin >> numMetal;
    metalPoints = vector<Point>(numMetal);

    for (int i = 0; i < numMetal; ++i)
        cin >> metalPoints[i].x >> metalPoints[i].y;

    for (int i = 0; i < NUM_RESOURCES; ++i)
        cin >> quotaAmounts[i];

    cin >> quotaMultiplier;

    cin >> numRows >> numCols;
    map = vector<vector<string>>(numRows, vector<string>(numCols));
    rewardMap = vector<vector<int>>(numRows, vector<int>(numCols));

    for (int i = 0; i < numCoal; ++i) {
        int x = coalPoints[i].x;
        int y = coalPoints[i].y;

        rewardMap[x][y] = COAL_REWARD;
    }

    for (int i = 0; i < numFish; ++i) {
        int x = fishPoints[i].x;
        int y = fishPoints[i].y;

        rewardMap[x][y] = FISH_REWARD;
    }

    for (int i = 0; i < numMetal; ++i) {
        int x = metalPoints[i].x;
        int y = metalPoints[i].y;

        rewardMap[x][y] = METAL_REWARD;
    }

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c)
            cin >> map[r][c];
    }
}

struct Node;

struct Node {
    int score;
    int stepAllowance;
    int row, col;
    Node *parent = nullptr;

    Node() { score = 0; };

    Node(int score, int row, int col, int stepAllowance,
         Node *parent = nullptr) {
        this->score = score;
        this->row = row;
        this->col = col;
        this->parent = parent;
        this->stepAllowance = stepAllowance;
    }

    bool operator==(const Node &other) {
        return this->row == other.row && this->col == other.col;
    }
};

class NodeCompare {
  public:
    bool operator()(Node *&node1, Node *&node2) {
        return node1->score < node2->score;
    }
};

void Solver::solve() {
    vector<pair<int, int>> neighbors = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));
    vector<vector<int>> cost(numRows, vector<int>(numCols, 0));

    Node *start = new Node(0, 0, 0, stepAllowance);
    Node *end = new Node(0, numRows - 1, numCols - 1, 0);

    priority_queue<Node *, vector<Node *>, NodeCompare> q;
    q.push(start);

    while (q.size()) {
        Node *top = q.top();
        q.pop();

        if (*top == *end) {
            if (top->score > end->score || end->parent == nullptr) {
                end->score = top->score;
                end->parent = top->parent;
            }
        }

        for (auto neighbor : neighbors) {
            int row = top->row + neighbor.first;
            int col = top->col + neighbor.second;

            if (row < 0 || row > numRows - 1 || col < 0 || col > numCols - 1)
                continue;

            if (visited[row][col])
                continue;

            visited[row][col] = true;

            string type = map[row][col];

            int score = top->score;
            int currAllowance = top->stepAllowance - 1;

            if (currAllowance < 0)
                score -= ALLOWANCE_NUMERATOR / travelDifficulty[type];
            else
                score += ALLOWANCE_NUMERATOR / travelDifficulty[type];

            score += rewardMap[row][col];
            Node *nb = new Node(score, row, col, currAllowance, top);
            cost[row][col] = max(cost[row][col], score);

            q.push(nb);
        }
    }

    vector<Node *> path;

    while (end != nullptr) {
        path.push_back(end);
        end = end->parent;
    }

    for (int i = path.size() - 1; i >= 0; --i) {
        printf("[%d, %d] ", path[i]->row, path[i]->col);
    }

    printf("\n");
    printf("COST MAP\n");
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            cout << cost[i][j] << " ";
        }

        printf("\n");
    }
}

void Solver::toString() {
    printf("Step Allowance: %d\n", stepAllowance);
    printf("Quotas: %d %d %d\n", quotaAmounts[0], quotaAmounts[1],
           quotaAmounts[2]);
    printf("Quota multiplier %f\n", quotaMultiplier);
    printf("Coal points: ");

    for (int i = 0; i < numCoal; ++i)
        cout << coalPoints[i].toString() << " ";

    printf("\n");

    printf("Fish points: ");

    for (int i = 0; i < numFish; ++i)
        cout << fishPoints[i].toString() << " ";

    printf("\n");

    printf("Metal points: ");

    for (int i = 0; i < numMetal; ++i)
        cout << metalPoints[i].toString() << " ";

    printf("\n");
    printf("Map size: %d x %d\n", numRows, numCols);

    printGrid(rewardMap);
}

void Solver::printGrid(vector<vector<int>> &grid) {
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (grid[i][j] > 0 && grid[i][j] <= 400)
                cout << grid[i][j] << "  ";
            else if (rewardMap[i][j] == 1000)
                cout << grid[i][j] << " ";
            else
                cout << grid[i][j] << "    ";
        }

        printf("\n");
    }
}