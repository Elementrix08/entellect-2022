#include "Path.h"

using namespace std;

Path::Path() { this->score = 0; }

Path::Path(vector<Node *> &path, int score) {
    this->path = path;
    this->score = score;
}

void Path::addPath(Path &other, vector<vector<bool>> &visited) {
    size_t n = other.path.size();

    for (int i = 1; i < n; ++i) {
        Node *point = other.path[i];
        visited[point->row][point->col] = true;
        path.push_back(point);
    }
}