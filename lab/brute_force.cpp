#include "brute_force.h"

// Перебор всех возможных подмножеств предметов
vector<int> brute_force(const vector<int>& knap_vec, const ll& target) {
  int sz = knap_vec.size();
  vector<int> solutions;
  for (int i = 1; i < (1 << sz); i++) {
    ll sum = 0;
    for (int j = 0; j < sz; j++) {
      if ((i >> j) & 1)
        sum += knap_vec[j];
    }
    if (sum == target)
      solutions.push_back(i);
  }
  return solutions;
}
