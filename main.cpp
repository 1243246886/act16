#include "ast-nodes.hpp"
#include "parser.hpp"
#include "flowgraph.hpp"
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

extern Program *myProgram;

int main()
{
    yyparse();

    cout << "Program entry is instruction " << myProgram->entry << endl;
    for (size_t i=0; i<myProgram->flist.size(); i++) {
	    Function* f = myProgram->flist[i];
	    cout << "Function " << i+1 << ": entry is " <<
		    f->entry << endl;
	    FlowGraph g(f);
	    for (size_t j=0; j<g.bb.size(); j++) {
		    int idx = g.bb[j];
		    cout << f->ilist[idx]->num << ' ';
	    }
	    cout << endl;
	    
	    for (size_t j=0; j<g.bb.size(); j++) {
		    int idx = g.bb[j];
		    cout << f->ilist[idx]->num << " -> ";
		    for (size_t k=0; k<g.edges[j].size(); k++) {
			    int idxt = g.bb[g.edges[j][k]];
			    cout << f->ilist[idxt]->num << ' ';
		    }
		    cout << endl;
	    }
	    cout << endl;
    }

    return 0;
}
