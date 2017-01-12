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

	yyparse();

	if (output_cfg) {
		for (size_t i=0; i<myProgram->flist.size(); i++) {
			Function* f = myProgram->flist[i];
			cout << "Function: " << f->entry << endl;
			FlowGraph g(f);
			cout << "Basic blocks: ";
			for (size_t j=0; j<g.bb.size(); j++) {
				int idx = g.bb[j];
				cout << f->ilist[idx]->num << ' ';
			}
			cout << endl << "CFG:" << endl;

			for (size_t j=0; j<g.bb.size(); j++) {
				int idx = g.bb[j];
				cout << f->ilist[idx]->num << " -> ";
				for (size_t k=0; k<g.edges[j].size(); k++) {
					int idxt = g.bb[g.edges[j][k]];
					cout << f->ilist[idxt]->num << ' ';
				}
				cout << endl;
			}
		}
	}

	return 0;
}
