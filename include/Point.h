#include <string>

struct Point {
  public:
    int x, y;
    Point(int x, int y);
    Point();

    std::string toString();
};