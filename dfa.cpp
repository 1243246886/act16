#include "dfa.hpp"
#include "flowgraph.hpp"
#include "ast-nodes.hpp"

#define VALID_VAR(value, fun) ((value<0 && -value<=fun->localspace) \
			       || (value>0 && value<=fun->paramspace+8))

void buildvmap(Function *f, DataFlowInfo &info)
{
	int nv = 0;
	map<string,int> &vmap = info.vmap;
	for (size_t i=0; i<f->ilist.size(); i++) {
		Instruction *ins = f->ilist[i];
		for (size_t j=0; j<2; j++) {
			Operand *op = ins->operands[j];
			if (op!=NULL) {
				if (op->operand_type==OPERAND_OFFSET
				    && VALID_VAR(op->v, f)
				    && vmap.find(op->name)==vmap.end()) {
					info.vmap[op->name] = nv;
					nv++;
				}
			}
		}
	}
}

void ins_defines(Function *f, DataFlowInfo &info)
{
	size_t len = f->ilist.size();

	info.defines.resize(len);
	for (size_t i=0; i<len; i++) {
		info.defines[i] = -1;
	}

	for (size_t i=0; i<len; i++) {
		Instruction *ins = f->ilist[i];
		if (ins->ins_type==INS_MOVE) {
			Operand *op = ins->operands[1];
			if (op->operand_type==OPERAND_OFFSET
			    && VALID_VAR(op->v, f)) {
				int vnum = info.vmap[op->name];
				info.defines[i] = vnum;
			}
		}
	}
}

void find_killset(Function *f, DataFlowInfo &info)
{
	size_t len = f->ilist.size();

	info.kill.resize(len);
	for (size_t i=0; i<len; i++) {
		info.kill[i] = set<int>();
		for (size_t j=0; j<len; j++) {
			if (j==i) continue;
			if (info.defines[j]==info.defines[i]) {
				info.kill[i].insert(j);
			}
		}
	}
}

void bbGenSet(FlowGraph &g, DataFlowInfo &info)
{
	size_t len = g.bb.size();
	info.gen.resize(len);
	for (size_t i=0; i<len; i++) {
		info.gen[i] = set<int>();
	}
	for (size_t i=0; i<len; i++) {
		int end = (i<len-1)?g.bb[i+1]:g.f->ilist.size();
		set<int> gs;
		gs.clear();
		for (int j=end-1; j>=g.bb[i]; j--) {
			int defv = info.defines[j];
			if (defv>=0) {
				if (gs.find(defv)==gs.end()) {
					info.gen[i].insert(j);
					gs.insert(defv);
				}
			}
		}
	}
}
