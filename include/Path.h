#include "Node.h"
#include <vector>

class Path {
  public:
    std::vector<Node *> path;
    int score;

    Path();
    Path(std::vector<Node *> &path, int score);
    void addPath(Path &other, std::vector<std::vector<bool>> &visited);
};