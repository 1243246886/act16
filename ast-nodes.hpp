#ifndef ACT16_AST_NODES_H
#define ACT16_AST_NODES_H

#include <vector>
#include <string>

using namespace std;

struct function;
struct instruction;
struct operand;

typedef struct program
{
	int entry;
	vector<struct function*> flist;
} Program;

typedef struct function
{
	int localspace;
	int paramspace;
	int entry;
	vector<struct instruction*> ilist;
} Function;

typedef enum {
	OPERAND_REG,
	OPERAND_LABEL,
	OPERAND_CONST,
	OPERAND_OFFSET,
	OPERAND_POINTER
} operand_t;

typedef struct operand
{
	operand_t operand_type;
	string name;
	int v;
} Operand;

typedef enum {
	INS_ENTER,
	INS_RET,
	INS_BR,
	INS_CONDBR,
	INS_DEFAULT
} ins_t;

typedef struct instruction
{
	int num;
	ins_t ins_type;
	int opcode;
	struct operand *operands[2];
	instruction();
	instruction(int n, int o){num=n; opcode=o; ins_type=INS_DEFAULT;}
} Instruction;

#endif
