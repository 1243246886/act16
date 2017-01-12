#include "ast-nodes.hpp"
#include "parser.hpp"

class FlowGraph
{
public:
	const Function *f;
	vector<int> bb;
	vector<vector<int> > edges;
	FlowGraph(const Function *f);
};
