#include <string>

struct Point {
  public:
    int row, col;
    Point(int row, int col);
    Point();

    std::string toString();
};