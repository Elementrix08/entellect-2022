#include "Point.h"
#include <map>
#include <vector>

class Solver {
  private:
    const static int NUM_RESOURCES = 3;
    const static int COAL_REWARD = 200;
    const static int FISH_REWARD = 400;
    const static int METAL_REWARD = 1000;
    const static int ALLOWANCE_NUMERATOR = 150;

    static std::map<std::string, int> travelDifficulty;

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
};