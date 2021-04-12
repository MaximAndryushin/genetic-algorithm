#include <bits/stdc++.h>

using namespace std;

// Константы
constexpr int SIZE = 24;
constexpr int NUMBER_OF_VECTORS = 50;
constexpr int NUMBER_OF_SOLUTIONS = 10;
constexpr int MIN_NUMBER_OF_ELEMENTS = 0.1 * SIZE; // округлил вниз
constexpr int MAX_NUMBER_OF_ELEMENTS = 0.5 * SIZE; // округление не нужно

// Объект, необходимый для случайной генерации числел
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

istream& operator>>(istream& in, vector<int>& vec) {
  char c;
  cin >> c;
  vec.resize(SIZE);
  for (int& v: vec) {
    cin >> v >> c;
  }
  return in;
}

int main() {
  freopen("zhopa.txt", "r", stdin);
  freopen("../gen_alg/lab/knapsack_problems.txt", "w", stdout);
  vector<int> id_sol(NUMBER_OF_SOLUTIONS);
  iota(id_sol.begin(), id_sol.end(), 1);
  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    vector<int> vec;
    vector<long long> weight;
    vector<float> elements_ratio;
    cin >> vec;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) {
      int number_of_elements = MIN_NUMBER_OF_ELEMENTS + rng() % (MAX_NUMBER_OF_ELEMENTS - MIN_NUMBER_OF_ELEMENTS + 1);
      set<int> st;
      int index;
      long long sum = 0;
      while(st.size() != number_of_elements) {
        index = rng() % SIZE;
        st.insert(index);
      }
      for (int i: st)
        sum += vec[i];
      weight.push_back(sum);
      elements_ratio.push_back((float)number_of_elements / SIZE);
    }
    for (int j: id_sol)
      cout << j << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++)
      cout << i + 1 << ' ';
    cout << endl;
    for (const auto& w: weight)
      cout << w << ' ';
    cout << endl;
    for (const auto& f: elements_ratio)
      cout << fixed << setprecision(2) << f << ' ';
    cout << endl;
  }
  return 0;
}

