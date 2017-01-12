#include "ast-nodes.hpp"
#include "parser.hpp"
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

extern Program *myProgram;

typedef struct
{
	vector<int> bb;
	vector<vector<int> > edges;
}FlowGraph;

void genFlowGraph(const Function *f, FlowGraph &g)
{
	g.bb.resize(0);
	g.edges.resize(0);
	const vector<Instruction*> &inslist = f->ilist;
	if (inslist.size()>0) {
		g.bb.push_back(0);
	} else {
		return;
	}

	map<int, int> insmap;

	for (size_t i=0; i<inslist.size(); i++) {
		insmap[inslist[i]->num] = i;
	}
	for (size_t i=1; i<inslist.size(); i++) {
		int tgt;
		switch (inslist[i]->opcode) {
		case BR:
		case BLBS:
		case BLBC:
			if (inslist[i]->opcode==BR) {
				tgt = inslist[i]->operands[0]->v;
			} else {
				tgt = inslist[i]->operands[1]->v;
			}
			g.bb.push_back(insmap[tgt]);
			/* fall through */
		case CALL:
			if (i+1<inslist.size()) {
				g.bb.push_back(i+1);
			}
			break;
		default:
			break;
		}
	}
	sort(g.bb.begin(), g.bb.end());
	vector<int>::iterator it;
	it = unique(g.bb.begin(), g.bb.end());
	g.bb.resize( std::distance(g.bb.begin(),it) );
}

int main()
{
    yyparse();

    cout << "Program entry is instruction " << myProgram->entry << endl;
    for (size_t i=0; i<myProgram->flist.size(); i++) {
	    Function* f = myProgram->flist[i];
	    cout << "Function " << i+1 << ": entry is " <<
		    f->entry << endl;
	    FlowGraph g;
	    genFlowGraph(f, g);
	    for (size_t j=0; j<g.bb.size(); j++) {
		    int idx = g.bb[j];
		    cout << f->ilist[idx]->num << ' ';
	    }
	    cout << endl;
    }

    return 0;
}
