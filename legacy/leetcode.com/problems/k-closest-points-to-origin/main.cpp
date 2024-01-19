#include "solution.hpp"

#include <iostream>
#include <vector>

using namespace std;
int main(int argc, char **argv) {
  vector<vector<int>> points;
  int cols = 2;
  int k, rows;
  std::cin >> k >> rows;
  for (int i = 0; i < rows; ++i) {
    vector<int> row;
    row.reserve(cols);
    for (int j = 0; j < cols; ++j) {
      int x;
      std::cin >> x;
      row.push_back(x);
    }
    points.push_back(std::move(row));
  }
  Solution solution;
  auto kc = solution.kClosest(points, k);
  cout << "K closest points are:\n";
  for (auto &p : kc) {
    cout << p[0] << ", " << p[1] << "\n";
  }
  return 0;
}
