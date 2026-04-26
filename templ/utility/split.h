#ifndef TEMPL_SPLIT_H
#define TEMPL_SPLIT_H

#include "bits/stdc++.h"

inline std::vector<std::string> split(const std::string& s, char d = ' ') {
  std::stringstream ss(s);
  std::string v;
  std::vector<std::string> ans;
  while (std::getline(ss, v, d))
    ans.push_back(v);
  return ans;
}

inline std::vector<int> split_int(const std::string& s, char d = ' ') {
  std::vector<int> ans;
  std::vector<std::string> spl = split(s, d);
  ans.reserve(spl.size());
  for (const std::string& v : spl)
    ans.push_back(std::stoi(v));
  return ans;
}

inline std::vector<long long> split_ll(const std::string& s, char d = ' ') {
  std::vector<long long> ans;
  std::vector<std::string> spl = split(s, d);
  ans.reserve(spl.size());
  for (const std::string& v : spl)
    ans.push_back(std::stoll(v));
  return ans;
}

#endif
