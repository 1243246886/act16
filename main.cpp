#include "ast-nodes.hpp"
#include "parser.hpp"
#include "flowgraph.hpp"
#include "dfa.hpp"
#include <iostream>
#include <map>
#include <algorithm>
#include <cstring>

using namespace std;

extern Program *myProgram;

int main(int argc, char *argv[])
{
	bool output_cfg = false;
	bool do_reachingdef = false;
	bool do_test = false;

	if (argc<2) {
		cerr << "Usage: " << argv[0] <<
			" <-opt|-backend=arg> < file" << endl;
		return 1;
	}

	if (strncmp(argv[1], "-backend", 8)==0) {
		if (strcmp(argv[1]+9, "cfg")==0) {
			output_cfg = true;
		}
	}

	if (strcmp(argv[1], "-opt=scp")==0) {
		do_reachingdef = true;
	}

	if (strcmp(argv[1], "-opt=test")==0) {
		do_test = true;
	}

	yyparse();
	vector<FlowGraph> g;
	for (size_t i=0; i<myProgram->flist.size(); i++) {
		g.push_back(FlowGraph(myProgram->flist[i]));
	}

	if (output_cfg) {
		for (size_t i=0; i<myProgram->flist.size(); i++) {
			Function* f = myProgram->flist[i];
			cout << "Function: " << f->entry << endl;
			cout << "Basic blocks: ";
			for (size_t j=0; j<g[i].bb.size(); j++) {
				int idx = g[i].bb[j];
				cout << f->ilist[idx]->num << ' ';
			}
			cout << endl << "CFG:" << endl;

			for (size_t j=0; j<g[i].bb.size(); j++) {
				int idx = g[i].bb[j];
				cout << f->ilist[idx]->num << " -> ";
				for (size_t k=0; k<g[i].edges[j].size(); k++) {
					int idxt = g[i].bb[g[i].edges[j][k]];
					cout << f->ilist[idxt]->num << ' ';
				}
				cout << endl;
			}
		}
	}

	if (do_test) {
		for (size_t i=0; i<myProgram->flist.size(); i++) {
			DataFlowInfo info;
			Function *fun = myProgram->flist[i];
			buildvmap(fun, info);
			ins_defines(fun, info);
			find_killset(fun, info);
			bbGenSet(g[i], info);
			map<string,int>::iterator it;
			for (it=info.vmap.begin(); it!=info.vmap.end();
			     it++) {
				cout << it->first << ' '
				     << it->second << endl;
			}
			for (size_t j=0; j<info.defines.size(); j++) {
				cout << fun->ilist[j]->num << ": "
				     << info.defines[j] << endl;
			}
			for (size_t j=0; j<g[i].bb.size(); j++) {
				int bbidx = g[i].bb[j];
				cout << "BB " << fun->ilist[bbidx]->num
				     << ": ";
				set<int>::iterator its;
				for (its=info.gen[j].begin();
				     its!=info.gen[j].end(); its++) {
					cout << *its << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}

	return 0;
}
