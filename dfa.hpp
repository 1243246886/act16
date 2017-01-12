#ifndef ACT16_DFA_H
#define ACT16_DFA_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

struct DataFlowInfo
{
	map<string,int> vmap;
	vector<int> defines;
	vector<set<int> > kill;
	vector<set<int> > gen;
};

#endif
