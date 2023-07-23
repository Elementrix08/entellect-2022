#include "Path.h"

using namespace std;

Path::Path() { this->score = 0; }

Path::Path(vector<Node *> &path, int score) {
    this->path = path;
    this->score = score;
}

void Path::addPath(Path &other) {
    size_t n = other.path.size();

    for (int i = 0; i < n; ++i) {
        Node *point = other.path[i];
        path.push_back(point);
    }
}