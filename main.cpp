#include "IMVector.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int AMOUNT = 10000;

int main() {
  auto start = std::chrono::system_clock::now();
  IMVector<int> v = IMVector<int>(100000);
  for (int i = 0; i < 100000; i++) {
    v = v.set(i, i);
    // cout << "CONTENTS" << endl;
    // cout << "END" << endl << endl << endl;
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  v.print_contents();
  cout << elapsed_seconds.count() << endl;
}
// v.print_contents();

//   IMVector<int> v2 = IMVector<int>(10000);
//   // IMVector<int> v2 = IMVector<int>();
//
//   // for (int i = 0; i < 100; i++) {
//
//   //  v2 = v2.push_back(i);
//   //}
//
//   for (int i = 0; i < 100; i++) {
//     v2 = v2.set(i, i);
//   }
//
//   v2.print_contents();
// }
/*
auto start2 = std::chrono::system_clock::now();
std::vector<int> ve = std::vector<int>();
for (int i = 0; i < AMOUNT; i++) {
  ve.push_back(i);
  // cout << "CONTENTS" << endl;
  // cout << "END" << endl << endl << endl;
}
auto end2 = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
cout << elapsed_seconds2.count() << endl;

cout << endl;
cout << endl;
cout << endl;
cout << endl;

auto t1 = elapsed_seconds.count();
auto t2 = elapsed_seconds2.count();

cout << t1 / t2 << endl;

auto start3 = std::chrono::system_clock::now();
std::unordered_map<int, int> um = std::unordered_map<int, int>();
for (int i = 0; i < AMOUNT; i++) {
  um[i] = i;
  // cout << "CONTENTS" << endl;
  // cout << "END" << endl << endl << endl;
}
auto end3 = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds3 = end3 - start3;
cout << elapsed_seconds3.count() << endl;

auto t3 = elapsed_seconds3.count();

cout << t1 / t3 << endl;

auto start4 = std::chrono::system_clock::now();
std::vector<std::pair<int, int>> vp = std::vector<std::pair<int, int>>();
for (int i = 0; i < 100000; i++) {
  vp.push_back(std::make_pair(i, i));
  // cout << "CONTENTS" << endl;
  // cout << "END" << endl << endl << endl;
}
auto end4 = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds4 = end4 - start4;
cout << elapsed_seconds4.count() << endl;

auto t4 = elapsed_seconds4.count();

cout << t1 / t4 << endl;

v = v.set(1, 12342);

// v.print_contents();

auto start5 = std::chrono::system_clock::now();
IMVector<IMVector<int>> vv = IMVector<IMVector<int>>();
for (int i = 0; i < 100; i++) {
  IMVector<int> vvv = IMVector<int>();
  vv = vv.push_back(vvv);
  for (int j = 0; j < 100; j++) {

    vv = vv.set(i, (vv.get(i)).push_back(j));
  }
}
auto end5 = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds5 = end5 - start5;
cout << elapsed_seconds5.count() << endl;

auto t5 = elapsed_seconds5.count();

cout << t1 / t5 << endl;

auto start6 = std::chrono::system_clock::now();
vector<vector<int>> asdf = vector<vector<int>>();
for (int i = 0; i < 100; i++) {
  vector<int> as = vector<int>();
  asdf.push_back(as);
  for (int j = 0; j < 100; j++) {
    asdf[i].push_back(j);
  }
}
auto end6 = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds6 = end6 - start6;
cout << elapsed_seconds6.count() << endl;

auto t6 = elapsed_seconds6.count();

cout << t5 / t6 << endl;
}
*/
