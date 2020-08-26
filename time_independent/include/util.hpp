#pragma once
#include <random>
#include <algorithm>

// for computation time
using Time = std::chrono::system_clock;


[[maybe_unused]]
static void halt(const std::string& msg)
{
  std::cout << "error: " << msg << std::endl;
  std::exit(1);
}

[[maybe_unused]]
static void halt(const std::string& where, const std::string& msg)
{
  std::cout << "error@" << where << ", " << msg << std::endl;
  std::exit(1);
}

[[maybe_unused]]
static void warn(const std::string& msg)
{
  std::cout << "warn: " << msg << std::endl;
}

// whether element 'a' is found in vector T
template <typename T>
static bool inArray(const T a, const std::vector<T> &arr) {
  auto itr = std::find(arr.begin(), arr.end(), a);
  return itr != arr.end();
}

template <typename T>
static void removeElement(T a, std::vector<T> &arr) {
  auto itr = std::find(arr.begin(), arr.end(), a);
  if (itr != arr.end()) arr.erase(itr);
}

template <typename T>
static std::vector<T> setMinus(std::vector<T> &arr1, std::vector<T> &arr2) {
  std::vector<T> result;
  for (auto e : arr1) if (!inArray(e, arr2)) result.push_back(e);
  return result;
}

template <typename T>
static std::vector<T> setUnion(std::vector<T> &arr1, std::vector<T> &arr2) {
  std::vector<T> result = arr1;
  for (auto e : arr2) if (!inArray(e, result)) result.push_back(e);
  return result;
}

template <typename T>
static std::vector<T> setUnion(std::vector<T> &arr1, T e) {
  std::vector<T> result = arr1;
  if (!inArray(e, result)) result.push_back(e);
  return result;
}

// return [from, to]
[[maybe_unused]]
static int getRandomInt(int from, int to, std::mt19937* MT) {
  std::uniform_int_distribution<int> r(from, to);
  return r(*MT);
}

[[maybe_unused]]
static int getRandomInt(int to, std::mt19937* MT) {
  return getRandomInt(0, to, MT);
}

// return [from, to)
[[maybe_unused]]
static float getRandomFloat(float from, float to, std::mt19937* MT) {
  std::uniform_real_distribution<float> r(from, to);
  return r(*MT);
}

// return one element randomly from vector
template <typename T>
static T randomChoose(std::vector<T> &arr, std::mt19937* MT) {
  return arr[getRandomInt(0, arr.size()-1, MT)];
}

// get elapsed time
[[maybe_unused]]
static double getElapsedTime(const std::chrono::system_clock::time_point&
                             t_start)
{
  Time::time_point t_end = Time::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>
    (t_end-t_start).count();
}
