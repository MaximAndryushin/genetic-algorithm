#include<bits/stdc++.h>
#include "brute_force.h"
#include "knapsack_gen_alg.h"
#include "knapsack_problem_generator.h"

// Функция вывода вектора в удобном формате
template<typename T>
ostream& operator<<(std::ostream& out, const vector<T>& obj) {
  out << '[';
  for (int i = 0; i < obj.size(); i++) {
    out << obj[i];
    if (i + 1 != obj.size()) 
      out << ", ";
  }
  out << ']' << endl;
  return out;
}

// Функция выводит индексы эл-ов, которые содержатся во множестве
void print_indices_of_set(int mask) {
  bool flag = 0;
  cout << '[';
  for (int i = 0; (1 << i) <= mask; i++) {
    if ((mask >> i) & 1) {
      if (flag)
        cout << ", ";
      cout << i + 1;
      flag = 1;
    }
  }
  cout << ']' << endl;
}

// Чтение рюкзачного вектора в формате [w1, w2, ..., wSIZE]
istream& operator>>(istream& in, vector<int>& vec) {
  char c;
  cin >> c;
  vec.resize(SIZE);
  for (int& v: vec) {
    cin >> v >> c;
  }
  return in;
}

void print_brute_solutions(const vector<vector<double>>& all, const vector<vector<double>>& first, const vector<vector<int>>& num) {
  freopen("../gen_alg/lab/brute_force_solutions.txt", "w", stdout);
  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << j + 1 << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << i + 1 << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << fixed << setprecision(6) << first[i][j] << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << fixed << setprecision(6) << all[i][j] << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << num[i][j] << ' ';
    cout << endl;
  }
  fclose(stdout);
}

void print_gen_alg_solutions(const vector<vector<double>>& t, const vector<vector<string>>& b_type, const vector<vector<int>>& num, const vector<vector<ll>>& f) {
  freopen("../gen_alg/lab/gen_alg_solutions.txt", "w", stdout);
  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << j + 1 << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << i + 1 << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << fixed << setprecision(6) << t[i][j] << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << f[i][j] << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << b_type[i][j] << ' ';
    cout << endl;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cout << num[i][j] << ' ';
    cout << endl;
  }
  fclose(stdout);
}

int main() {
  // Чтение рюкзачных векторов
  freopen("../gen_alg/lab/vectors_and_max_values.txt", "r", stdin);
  vector<vector<int>> knapsack_vectors(NUMBER_OF_VECTORS, vector<int>(SIZE));
  for (auto& vec: knapsack_vectors)
    cin >> vec;
  fclose(stdin);

  // Чтение целевых весов
  freopen("../gen_alg/lab/knapsack_problems.txt", "r", stdin);
  vector<vector<ll>> targets(NUMBER_OF_VECTORS, vector<ll>(NUMBER_OF_SOLUTIONS));
  for (auto& vec: targets) {
    int trash;
    float _trash;
    for (int i = 0; i < NUMBER_OF_SOLUTIONS; i++)
      cin >> trash;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++)
      cin >> trash;
    for (auto& t: vec)
      cin >> t;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++)
      cin >> _trash;
  }
  fclose(stdin);

  vector<vector<double>> times_of_full_solutions_brute(NUMBER_OF_VECTORS, vector<double>(NUMBER_OF_SOLUTIONS, 0));
  vector<vector<double>> times_of_first_solution_brute(NUMBER_OF_VECTORS, vector<double>(NUMBER_OF_SOLUTIONS, 0));
  vector<vector<int>> number_of_solutions(NUMBER_OF_VECTORS, vector<int>(NUMBER_OF_SOLUTIONS, 0));

  vector<vector<double>> times_of_solution_gen(NUMBER_OF_VECTORS, vector<double>(NUMBER_OF_SOLUTIONS, 0));
  vector<vector<string>> types_of_breakpoint(NUMBER_OF_VECTORS, vector<string>(NUMBER_OF_SOLUTIONS));
  vector<vector<int>> number_of_populations(NUMBER_OF_VECTORS, vector<int>(NUMBER_OF_SOLUTIONS));
  vector<vector<ll>> mins_of_fitness_function(NUMBER_OF_VECTORS, vector<ll>(NUMBER_OF_SOLUTIONS));

  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) {
      // Перебор
      {
        auto start = chrono::high_resolution_clock::now();
        auto [vec, first_solution_time] = brute_force(knapsack_vectors[i], targets[i][j]);
        auto end = chrono::high_resolution_clock::now();
        times_of_full_solutions_brute[i][j] = (chrono::duration_cast<chrono::duration<double>>(end - start)).count();
        times_of_first_solution_brute[i][j] = first_solution_time;
        number_of_solutions[i][j] = vec.size();
      }
    
      //Генетический алгоритм
      {
        auto start = chrono::high_resolution_clock::now();
        gen_ind::set_target(targets[i][j]);
        gen_ind::set_weights(knapsack_vectors[i]);
        auto [vec, type] = genetic_algorithm(times_of_full_solutions_brute[i][j]);
        auto end = chrono::high_resolution_clock::now();
        types_of_breakpoint[i][j] = enum_to_string(type);
        number_of_populations[i][j] = vec.size();
        times_of_solution_gen[i][j] = (chrono::duration_cast<chrono::duration<double>>(end - start)).count();
        mins_of_fitness_function[i][j] = vec.back().eval();
      }
    }
  }

  print_brute_solutions(
    times_of_full_solutions_brute,
    times_of_first_solution_brute,
    number_of_solutions
  );

  print_gen_alg_solutions(
    times_of_solution_gen,
    types_of_breakpoint,
    number_of_populations,
    mins_of_fitness_function
  );
}