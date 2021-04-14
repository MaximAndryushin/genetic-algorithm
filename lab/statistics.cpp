#include<bits/stdc++.h>
#include "knapsack_problem_generator.h"
#include "knapsack_gen_alg.h"

using namespace std;

int main() {
  // Чтение данных полного перебора
  freopen("../gen_alg/lab/brute_force_solutions.txt", "r", stdin);

  vector<double> mean_all_time_brute(NUMBER_OF_VECTORS, 0);
  vector<double> mean_first_time_brute(NUMBER_OF_VECTORS, 0);
  vector<double> varience_all_time_brute(NUMBER_OF_VECTORS, 0);
  vector<double> varience_first_time_brute(NUMBER_OF_VECTORS, 0);
  vector<double> st_dev_all_time_brute(NUMBER_OF_VECTORS, 0);
  vector<double> st_dev_first_time_brute(NUMBER_OF_VECTORS, 0);
  
  double val;

  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    vector<double> all_time;
    vector<double> first_time;
   
    for (int j = 0, trash; j < 2 * NUMBER_OF_SOLUTIONS; j++)
      cin >> trash;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) {
      cin >> val;
      first_time.push_back(val);
    }
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) {
      cin >> val;
      all_time.push_back(val);
    }
    for (int j = 0, trash; j < NUMBER_OF_SOLUTIONS; j++)
      cin >> trash;

    mean_all_time_brute[i] = accumulate(all_time.begin(), all_time.end(), (double)0) / all_time.size();
    mean_first_time_brute[i] = accumulate(first_time.begin(), first_time.end(), (double)0) / first_time.size();
    
    for (const auto& t: all_time)
      varience_all_time_brute[i] += (t - mean_all_time_brute[i]) * (t - mean_all_time_brute[i]);
    for (const auto& t: first_time)
      varience_first_time_brute[i] += (t - mean_first_time_brute[i]) * (t - mean_first_time_brute[i]);

    varience_all_time_brute[i] /= all_time.size();
    varience_first_time_brute[i] /= first_time.size();

    st_dev_all_time_brute[i] = sqrt(varience_all_time_brute[i]);
    st_dev_first_time_brute[i] = sqrt(mean_first_time_brute[i]);
  }

  fclose(stdin);

  // Чтение данных генетического алгоритма
  freopen("../gen_alg/lab/gen_alg_solutions.txt", "r", stdin);

  vector<double> mean_time_gen_alg(NUMBER_OF_VECTORS, 0);
  vector<double> varience_time_gen_alg(NUMBER_OF_VECTORS, 0);
  vector<double> st_dev_time_gen_alg(NUMBER_OF_VECTORS, 0);
  vector<int> number_of_solutions(NUMBER_OF_VECTORS, 0);

  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    vector<double> times;
    vector<char> good;
    vector<double> good_times;

    for (int j = 0, trash; j < 2 * NUMBER_OF_SOLUTIONS; j++)
      cin >> trash;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) {
      cin >> val;
      times.push_back(val);
    }
    for (int j = 0, fit_func; j < NUMBER_OF_SOLUTIONS; j++) {
      cin >> fit_func;
      good.push_back(fit_func == 0);
    }

    string _trash;
    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++) 
      cin >> _trash;
  
    for (int j = 0, trash; j < NUMBER_OF_SOLUTIONS; j++)
      cin >> trash;

    for (int j = 0; j < NUMBER_OF_SOLUTIONS; j++)
      if (good[j])
        good_times.push_back(times[j]);

    if (good_times.size() == 0) {
      mean_time_gen_alg[i] = varience_all_time_brute[i] = st_dev_all_time_brute[i] = -1;
      continue;
    }

    number_of_solutions[i] = good_times.size();

    mean_time_gen_alg[i] = accumulate(good_times.begin(), good_times.end(), (double)0) / good_times.size();
    
    for (const auto& t: good_times)
      varience_time_gen_alg[i] += (t - mean_time_gen_alg[i]) * (t - mean_time_gen_alg[i]);

    varience_time_gen_alg[i] /= good_times.size();

    st_dev_time_gen_alg[i] = sqrt(varience_time_gen_alg[i]);
  }

  fclose(stdin);

  // Запись статистики
  freopen("../gen_alg/lab/statistics.txt", "w", stdout);

  for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
    cout << SIZE << endl << MAX[i] << endl;
    cout << fixed << setprecision(6) << mean_first_time_brute[i] << ' ' << varience_first_time_brute[i] << ' ' << st_dev_first_time_brute[i] << endl;
    cout << fixed << setprecision(6) << mean_all_time_brute[i] << ' ' << varience_all_time_brute[i] << ' ' << st_dev_all_time_brute[i] << endl;
    cout << fixed << setprecision(6) << mean_time_gen_alg[i] << ' ' << varience_time_gen_alg[i] << ' ' << st_dev_time_gen_alg[i] << endl;
    cout << fixed << setprecision(2) << (double)number_of_solutions[i] / NUMBER_OF_SOLUTIONS << endl;
    cout << _POPULATION_SIZE << endl;
  }

  fclose(stdout);
}