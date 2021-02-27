#include <bits/stdc++.h>

#define endl "\n" 
using namespace std;
using ld = long double;

constexpr ld _MIN = 2; // минимальное значение аргумента
constexpr ld _MAX = 4; // максимальное значение аргумента
constexpr int _P = 50; // вероятность конвергенции и мутации в %
constexpr int _NUMBER_OF_POINTS = 1e6; // количество точек разбиения отрезка возможных значений аргумента
constexpr int _LEN = 20; // длина хромосомы в битах
constexpr int _POPULATION_SIZE = 20; // размер популяции

ld MIN, MAX; // минимальное/максимальное значение аргумента
int P; // вероятность конвергенции и мутации в %
int NUMBER_OF_POINTS;  // количество точек разбиения отрезка возможных значений аргумента
int LEN; // длина хромосомы в битах
int POPULATION_SIZE; // размер популяции

// объект, необходимый для случайнйо генерации числел
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// целевая/фитнесс функция
ld func(ld x) {
  return cos(exp(x)) / sin(log(x));
}

// переводит числа из кода Грея в бинарное представление
int to_bin(int x) {
  int n = 0;
	for (; x; x >>= 1)
		n ^= x;
	return n;
}

// переводит числа из бинарного представления в код Грея
int to_g(int x) {
  return x ^ (x >> 1);
}

// переводит число в бинарную строку
string to_str(int x) {
  string res;
  while(x) {
    res.push_back((x & 1) ? '1' : '0');
    x >>= 1;
  }
  reverse(res.begin(), res.end());
  return res;
}

// структура, представляющая особь
struct gen_ind {
  int gen; // представление хромосомы в виде кода Грея

  gen_ind() {
    gen = to_g(rng() % LEN);
  }

  gen_ind(int val) : gen(gen) {};

  // вычисление целевой (фитнесс) функции
  ld eval() const {
    return func(to_double());
  }

  // вычисление значения аргумента в виде числа с плавающей точкой из представления в виде кода Грея
  ld to_double() const {
    return MIN + to_bin(gen) * (MAX - MIN) / NUMBER_OF_POINTS;
  }

  bool operator <(const gen_ind& o) {
    return eval() < o.eval();
  }
};

// функция вывода объекта особи
ostream& operator<<(std::ostream& os, const gen_ind& obj) {
  cout << "Бинарное представление номера отрезка: " << to_str(to_bin(obj.gen)) << endl;
  cout << "Представление номера отрезка в виде кода Грея: " << to_str(obj.gen) << endl;
  cout << "Значения аргумента в виде числа с плавающей точкой: " << obj.to_double() << endl;
  cout << "Значения целевой (фитнесс) функции в виде числа с плавающей точкой: " << obj.eval() << endl;
  return os;
}

gen_ind genetic_algorithm() {
  // Вычисление длины хромосомы в битах
  LEN = 1;
  while(LEN < NUMBER_OF_POINTS)
    LEN <<= 1;
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  cout << fixed << setprecision(8);
  MIN = _MIN;
  MAX = _MAX;
  if (argc == 3) {
    P = atoi(argv[1]);
    NUMBER_OF_POINTS = atoi(argv[2]);
    POPULATION_SIZE = atoi(argv[3]);
  } else {
    P = _P;
    NUMBER_OF_POINTS = _NUMBER_OF_POINTS;
    POPULATION_SIZE = _POPULATION_SIZE;
  }

  cout << "Наилучшая особь: " << genetic_algorithm() << endl;
  
  return 0;
}
