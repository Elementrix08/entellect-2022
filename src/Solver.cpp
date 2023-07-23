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

void Solver::propogateReward(Node *reward) {
    const int REWARD_PROPOGATE_RADIUS = 2;

    vector<vector<bool>> visited(numRows, vector<bool>(numCols));
    queue<Node *> q;
    visited[reward->row][reward->col] = true;
    q.push(reward);

    int radius = 0;
    int currCount = 1, nextCount = 0;

    while (q.size()) {
        if (radius == REWARD_PROPOGATE_RADIUS) {
            return;
        }

        for (int i = 0; i < currCount; ++i) {
            Node *top = q.front();
            q.pop();

            for (auto neighbor : neighbors) {
                int row = top->row + neighbor.first;
                int col = top->col + neighbor.second;

                if (row < 0 || row > numRows - 1 || col < 0 ||
                    col > numCols - 1)
                    continue;

                if (visited[row][col])
                    continue;

                visited[row][col] = true;

                string type = map[row][col];

                int score = top->score;

                Node *nb = new Node(score, row, col, 0, top);

                rewardMap[row][col] =
                    max(rewardMap[row][col],
                        (score - radius) / reward->distanceTo(nb));
                q.push(nb);
                ++nextCount;
            }
        }

        currCount = nextCount;
        nextCount = 0;
        ++radius;
    }
}

void Solver::solve() {
    Node *start = new Node(0, 0, 0, stepAllowance);
    Node *end = new Node(0, numRows - 1, numCols - 1, 0);
    Node *currPosition = new Node(0, 0, 0, stepAllowance);
    Path finalPath;

    for (int i = 0; i < numCoal; ++i) {
        int row = coalPoints[i].row;
        int col = coalPoints[i].col;

        Node *point = new Node(COAL_REWARD, row, col, 0);
        propogateReward(point);
    }

    for (int i = 0; i < numFish; ++i) {
        int row = fishPoints[i].row;
        int col = fishPoints[i].col;

        Node *point = new Node(FISH_REWARD, row, col, 0);
        propogateReward(point);
    }

    for (int i = 0; i < numMetal; ++i) {
        int row = metalPoints[i].row;
        int col = metalPoints[i].col;

        Node *point = new Node(METAL_REWARD, row, col, 0);
        propogateReward(point);
    }

    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));

    if (*currPosition != *end) {
        Path endPath = goTo(currPosition, end);
        finalPath.addPath(endPath);
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
            cout << path[i][j] << "  ";
        }

        cout << "\n";
    }

    auto scores = calculateScore(finalPath.path);

    printf("\nScore = %lld - Penalties = %lld\n", scores.first, scores.second);

    printGrid(rewardMap);
}

void Solver::updateNodeScores(
    Node *currPosition,
    priority_queue<Node *, vector<Node *>, NodeCompare> &queue) {
    vector<Node *> nodes;
    Node *end = new Node(0, numRows - 1, numCols - 1, 0);

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            rewardMap[i][j] = 0;
        }
    }

    while (queue.size()) {
        Node *top = queue.top();
        queue.pop();

        Path path = goTo(currPosition, top);
        top->score = path.score;
        nodes.emplace_back(top);
    }

    for (Node *node : nodes) {
        propogateReward(node);
        queue.push(node);
    }
}

Path Solver::goTo(Node *start, Node *end) {
    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));

    priority_queue<Node *, vector<Node *>, NodeCompare> q;
    q.push(start);
    int pathAllowance = (start->stepAllowance == 0 ? 1 : start->stepAllowance);

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
            int step = pathAllowance - currAllowance;

            if (currAllowance < 0)
                score -= (ALLOWANCE_NUMERATOR / travelDifficulty[type]);
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
            cout << grid[i][j] << " ";
            // switch (grid[i][j]) {
            // case FISH_REWARD:
            //     cout << "F  ";
            //     break;
            // case METAL_REWARD:
            //     cout << "M  ";
            //     break;
            // case COAL_REWARD:
            //     cout << "C  ";
            //     break;
            // default:
            //     cout << "_  ";
            //     break;
            // }
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