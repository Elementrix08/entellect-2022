#include "Point.h"

Point::Point() {
    this->row = 0;
    this->col = 0;
}

Point::Point(int row, int col) {
    this->row = row;
    this->col = col;
}

std::string Point::toString() {
    return "[" + std::to_string(row) + "," + std::to_string(col) + "]";
}