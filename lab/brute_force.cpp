#include "brute_force.h"

// Перебор всех возможных подмножеств предметов
pair<vector<int>, double> brute_force(const vector<int>& knap_vec, const ll& target) {
  auto start = chrono::high_resolution_clock::now();
  double first_solution_time = 0;
  int sz = knap_vec.size();
  vector<int> solutions;
  for (int i = 1; i < (1 << sz); i++) {
    ll sum = 0;
    for (int j = 0; j < sz; j++) {
      if ((i >> j) & 1)
        sum += knap_vec[j];
    }
    if (sum == target) {
      if (solutions.size() == 0) {
        auto end = chrono::high_resolution_clock::now();
        first_solution_time = (chrono::duration_cast<chrono::duration<double>>(end - start)).count();
      } 
      solutions.push_back(i); 
    }
  }
  return { solutions, first_solution_time };
}
