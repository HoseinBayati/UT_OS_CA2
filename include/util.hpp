#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

using namespace std;

vector<string> split_string(const char *, char);
vector<string> get_resources();
vector<string> get_building_name();
vector<string> list_sub_dir(string path);

#endif