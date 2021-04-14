#include "knapsack_gen_alg.h"

int P; // Вероятность конвергенции и мутации в %
int LEN; // Длина хромосомы в битах
int POPULATION_SIZE; // Размер популяции
int POPULATION_NUMBER; // Номер популяции

// Объект, необходимый для случайной генерации числел
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

string enum_to_string(breakpoint_type t) {
  switch (t) {
    case ::time_limit:
      return "time";
    case ::iterations: 
      return "iterations";
    case ::zero:
      return "zero";
  }
}

// Переводит число в бинарную строку
string to_str(int x) {
  string res;
  while(x) {
    res.push_back((x & 1) ? '1' : '0');
    x >>= 1;
  }
  reverse(res.begin(), res.end());
  return res;
}

// Определение методов структуры gen_ind

// Восстановление хромосомы (сумма весов предметов <= target) ЖАДНОЕ
void gen_ind::truncate() {
  ll sum = to_sum();
  if (sum <= gen_ind::target)
    return;
  vector<int> id;
  for (int i = 0; (1 << i) <= gen; i++)
    if ((gen >> i) & 1)
      id.push_back(i);
  sort(id.begin(), id.end(), [&](int a, int b) {
    return gen_ind::weights[a] > gen_ind::weights[b];
  });
  while(sum > gen_ind::target) {
    int i = id.back();
    sum -= weights[i];
    id.pop_back();
  }
  gen = 0;
  for (int i: id)
    gen |= (1 << i);
}

// Генерация случайной хромосомы
gen_ind::gen_ind() {
  gen = rng() % (1 << LEN);
  truncate();
}

// Вычисление фитнесс функции
ll gen_ind::eval() const {
  return fitness_function(to_sum());
}

// Фитнесс функция
ll gen_ind::fitness_function(ll sum) const {
  return sum - target;
}

// Сохранение рюкзачного вектора
void gen_ind::set_weights(const vector<int>& w) {
  weights = w;
}

// Сохранение целевого веса
void gen_ind::set_target(ll t) {
  target = t;
}

// Вычисление суммы весов предметов
ll gen_ind::to_sum() const {
  ll sum = 0;
  for (int i = 0; (1 << i) <= gen; i++)
    if ((gen >> i) & 1)
      sum += weights[i];
  return sum;
}

// Мутация хромосомы (изменение случайного бита)
void gen_ind::mutate() {
  gen ^= (1 << (rng() % (LEN + 1)));
  truncate();
}

// Кроссинговер хромосомы
void gen_ind::crossingover(const gen_ind& o) {
  int id = rng() % LEN;
  for (int i = id; i <= LEN; i++) {
    gen = gen ^ (gen & (1 << id)) ^ (o.gen & (1 << id));
  }
  truncate();
}

bool gen_ind::operator<(const gen_ind& o) const {
  ll l = eval();
  ll r = o.eval();
  if (l != r)
    return l < r;
  return get() < o.get();
}

bool gen_ind::operator>(const gen_ind& o) const {
  ll l = eval();
  ll r = o.eval();
  if (l != r)
    return l > r;
  return get() > o.get();
}

// Функция вывода объекта особи
ostream& operator<<(std::ostream& os, const gen_ind& obj) {
  cout << "Вектор индикатор: " << to_str(obj.get()) << endl;
  cout << "Сумма весов предметов, входящих в множество: " << obj.to_sum() << endl;
  cout << "Значение фитнесс функции: " << obj.eval() << endl;
  return os;
}

// Репродукция
void reproduction(vector<gen_ind>& pop) {
  if (REPRODUCTION) {
    vector<gen_ind> new_pop;
    ll sum = 0;
    int count;
    for (const auto& g: pop)
      sum += g.eval();
    for (const auto& g: pop) {
      count = round(g.eval() / sum * POPULATION_SIZE);
      for (int i = 0; i < count; i++)
        new_pop.push_back(g);
    }
    swap(new_pop, pop);
  }
}

// Кроссинговер
void crossingover(vector<gen_ind>& pop) {
  for (int i = ELITE, j; i < POPULATION_SIZE; i++) {
    if (rng() % 100 < P) {
      j = rng() % POPULATION_SIZE;
      if (i == j)
        j = (j - 1 + POPULATION_SIZE) % POPULATION_SIZE;
      pop[i].crossingover(pop[j]);
    }
  }
}

// Мутация
void mutation(vector<gen_ind>& pop) {
  for (auto& gen: pop) 
    if (rng() % 100 < P)
      gen.mutate();
}

// возвращает лучшие особи для каждой популяцииы
pair<vector<gen_ind>, breakpoint_type> genetic_algorithm(double time) {
  auto start = chrono::high_resolution_clock::now();
  // Инициализация необходимых параметров
  LEN = gen_ind::weights.size();
  POPULATION_SIZE = _POPULATION_SIZE;
  P = _P;
  
  // Генерация начальной популяции
  vector<gen_ind> pop(POPULATION_SIZE);
  vector<gen_ind> answer;

  /* 
  Точка останова
   (если это значение станет равным BREAKPOINT, алгоритм заканчивает работу)
   (т.е. алгоритм не будет улучшать ответ в течении BREAKPOINT итераций)
  */
  int num = 0; 
  POPULATION_NUMBER = 0; // число поколений (генераций)

  gen_ind res;

  do {
    POPULATION_NUMBER++;

    // Сортировка особей по значению целевой функции
    sort(pop.begin(), pop.end(), greater<>());

    // Удаление дубликатов
    pop.resize(distance(pop.begin(), unique(pop.begin(), pop.end())));

    // Добавление случайных особей до полной популяции
    while(pop.size() < POPULATION_SIZE)
      pop.emplace_back();

    // Проверка условия остановки
    if (res.eval() >= pop.front().eval())
      num++;
    else {
      res = pop.front();
      num = 0;
    }
    answer.emplace_back(pop.front());

    // Репродукция
    reproduction(pop);

    // Кроссинговер
    crossingover(pop);

    // Мутация
    mutation(pop);
    
    // Проверка значения фитнесс функции
    if (res.eval() == 0)
      return { answer, ::zero };

    auto end = chrono::high_resolution_clock::now();
    if ((chrono::duration_cast<chrono::duration<double>>(end - start)).count() > 2 * time)
      return { answer, ::time_limit };

  } while(num != BREAKPOINT); // Проверка условия остановки алгоритма

  swap(answer.back(), res);
  return { answer, ::iterations };
}
