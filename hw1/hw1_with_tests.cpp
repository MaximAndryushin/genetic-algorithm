#include <bits/stdc++.h>

#define endl "\n"
#define REPRODUCTION 0
using namespace std;
using ld = long double;

constexpr ld _MIN = 2; // Минимальное значение аргумента
constexpr ld _MAX = 4; // Максимальное значение аргумента
constexpr int _P = 50; // Вероятность конвергенции и мутации в %
constexpr int _NUMBER_OF_POINTS = 1 << 20; // Количество точек разбиения отрезка возможных значений аргумента
constexpr int _LEN = 20; // Длина хромосомы в битах
constexpr int _POPULATION_SIZE = 20; // Размер популяции
constexpr int BREAKPOINT = 10; // Условие остановки алгоритма
constexpr int ELITE = 5; // Количесвто особей, не подврегающихся конвергенции

ld MIN, MAX; // Минимальное/максимальное значение аргумента
int P; // Вероятность конвергенции и мутации в %
int NUMBER_OF_POINTS;  // Количество точек разбиения отрезка возможных значений аргумента
int LEN; // Длина хромосомы в битах
int POPULATION_SIZE; // Размер популяции
int POPULATION_NUMBER;


// Объект, необходимый для случайной генерации числел
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// Целевая/фитнесс функция
ld func(ld x) {
  return cos(exp(x)) / sin(log(x));
}

// Переводит числа из кода Грея в бинарное представление
int to_bin(int x) {
  int n = 0;
	for (; x; x >>= 1)
		n ^= x;
	return n;
}

// Переводит числа из бинарного представления в код Грея
int to_g(int x) {
  return x ^ (x >> 1);
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

// Структура, представляющая особь
struct gen_ind {
  gen_ind() {
    gen = to_g(rng() % LEN);
  }

  gen_ind(int gen) : gen(gen) {};

  int get() const {
    return gen;
  }

  // Вычисление целевой (фитнесс) функции
  ld eval() const {
    return func(to_double());
  }

  // Вычисление значения аргумента в виде числа с плавающей точкой из представления в виде кода Грея
  ld to_double() const {
    return MIN + to_bin(gen) * (MAX - MIN) / NUMBER_OF_POINTS;
  }

  // Мутация хромосомы (изменение случайного бита)
  void mutate() {
    gen ^= (1 << (rng() % (LEN + 1)));
  }

  // Кроссинговер хромосомы
  void crossingover(const gen_ind& o) {
    int id = rng() % LEN;
    for (int i = id; i <= LEN; i++) {
      gen = gen ^ (gen & (1 << id)) ^ (o.gen & (1 << id));
    }
  }

  bool operator <(const gen_ind& o) const {
    return eval() < o.eval();
  }

  bool operator >(const gen_ind& o) const {
    return eval() > o.eval();
  }

  bool operator ==(const gen_ind& o) const {
    return gen == o.gen;
  }

  private:

  int gen; // Представление хромосомы в виде кода Грея
};

// Функция вывода объекта особи
ostream& operator<<(std::ostream& os, const gen_ind& obj) {
  cout << "Бинарное представление номера отрезка: " << to_str(to_bin(obj.get())) << endl;
  cout << "Представление номера отрезка в виде кода Грея: " << to_str(obj.get()) << endl;
  cout << "Номера отрезка: " << to_bin(obj.get()) << endl;
  cout << "Значения аргумента в виде числа с плавающей точкой: " << obj.to_double() << endl;
  cout << "Значения целевой (фитнесс) функции в виде числа с плавающей точкой: " << obj.eval() << endl;
  return os;
}

// Функция вывода вектора в представлении листа для питона
template<typename T>
ostream& operator<<(std::ostream& out, const vector<T>& obj) {
  out << '[';
  for (const auto& v: obj)
    out << v << ", ";
  out << "\b\b";
  out << ']' << endl;
  return out;
}

// Репродукция
void reproduction(vector<gen_ind>& pop) {
  if (REPRODUCTION) {
    vector<gen_ind> new_pop;
    ld sum = 0;
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

gen_ind genetic_algorithm() {
  // Вычисление длины хромосомы в битах
  LEN = 0;
  while((1 << LEN) < NUMBER_OF_POINTS)
    LEN++;

  NUMBER_OF_POINTS = 1 << LEN;
  
  // Генерация начальной популяции
  vector<gen_ind> pop(POPULATION_SIZE);

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

    // Репродукция
    reproduction(pop);

    // Кроссинговер
    crossingover(pop);

    // Мутация
    mutation(pop);

  } while(num != BREAKPOINT); // Проверка условия остановки алгоритма

  return res;
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  cout << fixed << setprecision(8);

  MIN = _MIN;
  MAX = _MAX;
  if (argc == 4) {
    P = atoi(argv[1]);
    NUMBER_OF_POINTS = atoi(argv[2]);
    POPULATION_SIZE = atoi(argv[3]);
  } else {
    P = _P;
    NUMBER_OF_POINTS = _NUMBER_OF_POINTS;
    POPULATION_SIZE = _POPULATION_SIZE;
  }

  int POP = 100;
  int REPEATS = 20;
  vector<ld> maxs, times, pops;
  vector<int> sizes;
  for (int i = 1; i <= POP; i++) {
    sizes.push_back(i);
    POPULATION_SIZE = i;
    ld ans = 0;
    ld t = 0;
    ld pop_num = 0;
    for (int j = 0; j < REPEATS; j++) {
      auto start = chrono::high_resolution_clock::now();
      ans += genetic_algorithm().eval();
      auto end = chrono::high_resolution_clock::now();
      t += (chrono::duration_cast<chrono::duration<double>>(end - start)).count();
      pop_num += POPULATION_NUMBER;
    }
    maxs.push_back(ans / REPEATS);
    times.push_back(t / REPEATS);
    pops.push_back(pop_num / REPEATS);
    //cout << "Наилучшая особь: " << endl << genetic_algorithm() << endl;
  }

  cout << sizes << maxs << times << pops;
  
  // ld real_ans = -1e9, val; 
  // int id = 0;
  // for (int i = 0; i <= NUMBER_OF_POINTS; i++) {
  //   val = func(MIN + i * (MAX - MIN) / NUMBER_OF_POINTS);
  //   if (real_ans < val)
  //     real_ans = val, id = i;
  // }
  // cout << "Лучший ответ при такой точности: " << endl << gen_ind(to_g(id));

  return 0;
}
