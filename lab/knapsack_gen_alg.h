#ifndef __KNAPSACK_GEN_ALG_H_INCLUDED__
#define __KNAPSACK_GEN_ALG_H_INCLUDED__
#include <bits/stdc++.h>

#define REPRODUCTION 0
using namespace std;
using ll = long long;

constexpr int _P = 40; // Вероятность конвергенции и мутации в %
constexpr int _POPULATION_SIZE = 30; // Размер популяции
constexpr int BREAKPOINT = 15; // Условие остановки алгоритма
constexpr int ELITE = 5; // Количесвто особей, не подврегающихся конвергенции

// Фитнесс-функция
ll func();

// Структура, представляющая особь
struct gen_ind {
  // Рюкзачный вектор
  inline static vector<int> weights = {};

  // Целевой вес
  inline static ll target = 0;

  gen_ind(int gen) : gen(gen) { truncate(); };

  // Генерация случайной хромосомы
  gen_ind();

  int get() const { return gen; }

  // Сохранение рюкзачного вектора
  static void set_weights(const vector<int>& w);

  // Сохранение целевого веса
  static void set_target(ll t);

  // Вычисление суммы весов предметов
  ll to_sum() const;

  // Вычисление целевой (фитнесс) функции
  ll eval() const;

  // Фитнесс функция
  ll fitness_function(ll sum) const;

  // Мутация хромосомы (изменение случайного бита)
  void mutate();

  // Кроссинговер хромосомы
  void crossingover(const gen_ind& o);

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
  // Представление хромосомы в виде вектора индикатора (i-ый бит равен 1, если предмет включен во множество)
  int gen;

  // Восстановление хромосомы (сумма весов предметов <= target) ЖАДНОЕ
  void truncate();
};

// Функция вывода объекта особи
ostream& operator<<(std::ostream& os, const gen_ind& obj);

// Репродукция
void reproduction(vector<gen_ind>& pop);

// Кроссинговер
void crossingover(vector<gen_ind>& pop);

// Мутация
void mutation(vector<gen_ind>& pop);

// возвращает лучшие особи для каждой популяцииы
vector<gen_ind> genetic_algorithm();


#endif