#ifndef pSS_h
#define pSS_h

#include <ctype.h>
#include <omp.h>

#include <algorithm>
#include <bitset>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <thread>
#include <vector>
using namespace std;

random_device rand_dev;
mt19937 gen(rand_dev());

void randPermutation(vector<int>&);
int greedyColoring(vector<vector<int>>&, vector<int>&, vector<int>&);
int fast_recoloring(vector<vector<int>>&, vector<int>&, vector<int>&,int);
int SS(vector<vector<int>>&, int );
bool readfile(string fileName, vector<vector<int>>&, int&, int&);
#endif
