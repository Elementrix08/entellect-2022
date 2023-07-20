#include "Point.h"

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

std::string Point::toString() {
    return "[" + std::to_string(x) + "," + std::to_string(y) + "]";
}