#include "Node.h"
#include "Path.h"
#include "Point.h"

#include <map>
#include <queue>
#include <vector>

class Solver {
  private:
    const static int NUM_RESOURCES = 3;
    const static int COAL_REWARD = 200;
    const static int FISH_REWARD = 400;
    const static int METAL_REWARD = 1000;
    const static int ALLOWANCE_NUMERATOR = 150;
    static std::map<std::string, int> travelDifficulty;
    const std::vector<std::pair<int, int>> neighbors = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  public:
    int stepAllowance;
    float quotaMultiplier;
    int quotaAmounts[NUM_RESOURCES];

    int numCoal;
    std::vector<Point> coalPoints;

    int numFish;
    std::vector<Point> fishPoints;

    int numMetal;
    std::vector<Point> metalPoints;

    int numRows, numCols;
    std::vector<std::vector<std::string>> map;
    std::vector<std::vector<int>> rewardMap;

    Solver();
    void solve();
    void toString();
    void printGrid(std::vector<std::vector<int>> &grid);
    void propogateReward(Node *reward);
    std::pair<long long, long long> calculateScore(std::vector<Node *> &path);
    Path goTo(Node *start, Node *end);
};