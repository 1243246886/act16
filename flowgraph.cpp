#include "flowgraph.hpp"
#include <map>
#include <algorithm>

using namespace std;

static
void genFlowGraph(const Function *f, FlowGraph &g)
{
	g.bb.resize(0);

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

	/* first step: identify basic blocks */
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

	g.edges.resize(g.bb.size());
	for (size_t i=0; i<g.bb.size(); i++) {
		g.edges[i] = vector<int>(0);
	}
	/* step two: connect the blocks */
	for (size_t i=0; i<g.bb.size()-1; i++) {
		Instruction *last = f->ilist[g.bb[i+1]-1];
		ins_t lastins = last->ins_type;
		if (lastins!=INS_BR) {
			g.edges[i].push_back(i+1);
		}
		int tgt = -1;
		if (lastins==INS_BR) {
			tgt = insmap[last->operands[0]->v];
		} else if (lastins==INS_CONDBR) {
			tgt = insmap[last->operands[1]->v];
		}
		if (tgt!=-1) {
			for (size_t j=0; j<g.bb.size(); j++) {
				if (g.bb[j]==tgt) {
					g.edges[i].push_back(j);
					break;
				}
			}
		}
	}
}

FlowGraph::FlowGraph(const Function *f)
{
	this->f = f;
	genFlowGraph(f, *this);
}
