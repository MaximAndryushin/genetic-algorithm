#ifndef __BRUTE_FORCE_H_INCLUDED__
#define __BRUTE_FORCE_H_INCLUDED__
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// Функция выводит индексы эл-ов, которые содержатся во множестве
void print_indices_of_set(int mask);

// Перебор всех возможных подмножеств предметов
std::vector<int> brute_force(const std::vector<int>& knap_vec, const long long& target);

#endif