#include "dfa.hpp"
#include "ast-nodes.hpp"

#define VALID_VAR(value, fun) ((value<0 && -value<=fun->localspace) \
			       || (value>0 && value<=fun->paramspace+8))

void buildvmap(Function *f, DataFlowInfo &info)
{
	int nv = 0;
	for (size_t i=0; i<f->ilist.size(); i++) {
		Instruction *ins = f->ilist[i];
		for (size_t j=0; j<2; j++) {
			Operand *op = ins->operands[i];
			if (op!=NULL) {
				if (op->operand_type==OPERAND_OFFSET) {
					if (VALID_VAR(op->v, f)) {
						info.vmap[op->name] = nv;
						nv++;
					}
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
