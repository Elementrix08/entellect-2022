#include "Solver.h"
#include <algorithm>
#include <cmath>
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
        cin >> coalPoints[i].row >> coalPoints[i].col;

    cin >> numFish;
    fishPoints = vector<Point>(numFish);

    for (int i = 0; i < numFish; ++i)
        cin >> fishPoints[i].row >> fishPoints[i].col;

    cin >> numMetal;
    metalPoints = vector<Point>(numMetal);

    for (int i = 0; i < numMetal; ++i)
        cin >> metalPoints[i].row >> metalPoints[i].col;

    for (int i = 0; i < NUM_RESOURCES; ++i)
        cin >> quotaAmounts[i];

    cin >> quotaMultiplier;

    cin >> numRows >> numCols;
    map = vector<vector<string>>(numRows, vector<string>(numCols));
    rewardMap = vector<vector<int>>(numRows, vector<int>(numCols));

    for (int i = 0; i < numCoal; ++i) {
        int row = coalPoints[i].row;
        int col = coalPoints[i].col;

        rewardMap[row][col] = COAL_REWARD;
    }

    for (int i = 0; i < numFish; ++i) {
        int row = fishPoints[i].row;
        int col = fishPoints[i].col;

        rewardMap[row][col] = FISH_REWARD;
    }

    for (int i = 0; i < numMetal; ++i) {
        int row = metalPoints[i].row;
        int col = metalPoints[i].col;

        rewardMap[row][col] = METAL_REWARD;
    }

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c)
            cin >> map[r][c];
    }
}

void Solver::solve() {
    Node *start = new Node(0, 0, 0, stepAllowance);
    Node *end = new Node(0, numRows - 1, numCols - 1, 0);
    Node *currPosition = new Node(0, 0, 0, stepAllowance);
    Path finalPath;
    finalPath.path.push_back(start);

    priority_queue<Node *, vector<Node *>, NodeCompare> promisingNodes;

    for (int i = 0; i < numCoal; ++i) {
        int row = coalPoints[i].row;
        int col = coalPoints[i].col;

        Node *point = new Node(0, row, col, 0);
        promisingNodes.push(point);
    }

    for (int i = 0; i < numFish; ++i) {
        int row = fishPoints[i].row;
        int col = fishPoints[i].col;

        Node *point = new Node(0, row, col, 0);
        promisingNodes.push(point);
    }

    for (int i = 0; i < numMetal; ++i) {
        int row = metalPoints[i].row;
        int col = metalPoints[i].col;

        Node *point = new Node(0, row, col, 0);
        promisingNodes.push(point);
    }

    updateNodeScores(currPosition, promisingNodes);

    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));
    int scoreToEnd = 0;

    while (promisingNodes.size()) {
        Node *top = promisingNodes.top();
        promisingNodes.pop();

        if (visited[top->row][top->col])
            continue;

        int nextBestScore = top->score / (currPosition->stepAllowance -
                                          currPosition->distanceTo(top));

        if (currPosition->distanceTo(end) > nextBestScore) {
            Path endPath = goTo(currPosition, end);
            finalPath.addPath(endPath, visited);
            break;
        }

        Path pathToMaterial = goTo(currPosition, top);
        finalPath.addPath(pathToMaterial, visited);
        currPosition->row = top->row;
        currPosition->col = top->col;
        currPosition->stepAllowance -= pathToMaterial.path.size();

        updateNodeScores(currPosition, promisingNodes);
    }

    vector<vector<char>> path(numRows, vector<char>(numCols, '_'));
    printf("SOLUTION\n");

    for (Node *node : finalPath.path) {
        printf("[%d, %d] ", node->row, node->col);
        path[node->row][node->col] = '*';
    }

    printf("\n");

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            cout << path[i][j] << " ";
        }

        cout << "\n";
    }

    auto scores = calculateScore(finalPath.path);

    printf("\nScore = %lld - Penalties = %lld\n", scores.first, scores.second);
}

void Solver::updateNodeScores(
    Node *currPosition,
    priority_queue<Node *, vector<Node *>, NodeCompare> &queue) {
    vector<Node *> nodes;

    while (queue.size()) {
        Node *top = queue.top();
        queue.pop();

        Path path = goTo(currPosition, top);
        top->score = path.score;
        nodes.emplace_back(top);
    }

    for (Node *node : nodes) {
        queue.push(node);
    }
}

Path Solver::goTo(Node *start, Node *end) {
    vector<pair<int, int>> neighbors = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));

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
            nb->score -= nb->distanceTo(end);

            q.push(nb);
        }
    }

    vector<Node *> path;
    int score = end->score;

    while (end != nullptr) {
        path.push_back(end);
        end = end->parent;
    }

    reverse(path.begin(), path.end());
    return Path(path, score);
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

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (map[i][j].length() == 2)
                cout << map[i][j] << " ";
            else
                cout << map[i][j] << "  ";
        }

        printf("\n");
    }
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

pair<long long, long long> Solver::calculateScore(vector<Node *> &path) {
    vector<vector<int>> materialRewards(numRows, vector<int>(numCols, 0));

    for (int i = 0; i < numCoal; ++i) {
        int row = coalPoints[i].row, col = coalPoints[i].col;
        materialRewards[row][col] = COAL_REWARD;
    }

    for (int i = 0; i < numFish; ++i) {
        int row = fishPoints[i].row, col = fishPoints[i].col;
        materialRewards[row][col] = FISH_REWARD;
    }

    for (int i = 0; i < numMetal; ++i) {
        int row = metalPoints[i].row, col = metalPoints[i].col;
        materialRewards[row][col] = METAL_REWARD;
    }

    long long score = materialRewards[0][0];
    long long penalties = 0;

    for (int i = 1; i < path.size(); ++i) {
        int row = path[i]->row;
        int col = path[i]->col;
        string tileDiff = map[row][col];

        if (i <= stepAllowance) {
            score += (ALLOWANCE_NUMERATOR / travelDifficulty[tileDiff]) /
                     (erf((i - 1) / stepAllowance) + 1);
        } else {
            score -= (ALLOWANCE_NUMERATOR / travelDifficulty[tileDiff]) *
                     (erf((i - 1) / stepAllowance) + 1);
            penalties += (ALLOWANCE_NUMERATOR / travelDifficulty[tileDiff]) *
                         (erf((i - 1) / stepAllowance) + 1);
        }

        score += materialRewards[row][col];
    }

    return {score, penalties};
}