#include "Point.h"
#include <vector>

class Solver {
  private:
    const static int NUM_RESOURCES = 3;

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

    Solver();
    void toString();
};