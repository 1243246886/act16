#ifndef ACT16_DFA_H
#define ACT16_DFA_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "ast-nodes.hpp"
#include "flowgraph.hpp"

using namespace std;

struct DataFlowInfo
{
	/* variable id to integer mapping */
	map<string,int> vmap;
	/* the variable each instruction defines */
	vector<int> defines;
	/* kill set of each instruction */
	vector<set<int> > kill;
	/* gen set for a basic block,
	   store the instruction index
	   of the function
	*/
	vector<set<int> > gen;
};

void buildvmap(Function *f, DataFlowInfo &info);
void ins_defines(Function *f, DataFlowInfo &info);
void find_killset(Function *f, DataFlowInfo &info);
void bbGenSet(FlowGraph &g, DataFlowInfo &info);

#endif
