%{
#include "ast-nodes.hpp"
#include <cstdio>
#include <cstdlib>

Program *myProgram;
extern int yylex();
void yyerror(const char *s) { std::printf("Error: %s\n", s); std::exit(1); }
%}

%union{
    Program *program;
    Function *func;
    Instruction *ins;
    Operand *operand;
    std::string *str;
    vector<Instruction*> *instr_list;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <token> INSTR
%token <str> INTEGER REG INS_LB IDENTIFIER
%token <token> ADD SUB MUL DIV MOD NEG CMPEQ CMPLE CMPLT
%token <token> NOP
%token <token> BR
%token <token> BLBC BLBS
%token <token> CALL
%token <token> LOAD
%token <token> STORE
%token <token> READ
%token <token> WRITE
%token <token> WRL
%token <token> PARAM
%token <token> ENTER RET ENTRYPC
%token <token> SHARP COLON

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */

%type <ins> common_instr enter_instr ret_instr common_real_instr
%type <func> function entry_function
%type <token> label integer op_arith op_condbr
%type <program> program
%type <operand> operand offset_exp reg_operand lb_operand
%type <instr_list> instr_list

%start program

%%

program: /* empty */ { myProgram = new Program; }
       | program function { $$->flist.push_back($2); }
       | program entry_function { $$->flist.push_back($2); }
       | program common_instr
       ;

common_instr:
            INSTR label common_real_instr 
            { 
            $$ = $3;
            $$->num = $2;
            }
            ;

enter_instr:
           INSTR label ENTER integer { $$ = new Instruction($2, $4); }
           ;

ret_instr:
         INSTR label RET integer { $$ = new Instruction($2, $4); }
         ;

entrypc_instr:
             INSTR label ENTRYPC

label:
     integer COLON { $$=$1; }

instr_list: /* empty */ { $$ = new vector<Instruction*>; }
          | instr_list common_instr { $$ = $1; $$->push_back($2); }
          ;

function:
        enter_instr instr_list ret_instr { $$ = new Function; }
        ;

entry_function:
              entrypc_instr function { $$=$2; }

common_real_instr:
                 NOP { $$ = new Instruction(0, $1); }
                 | op_arith operand operand { 
                 $$ = new Instruction(0, $1); 
                 $$->operands[0] = $2;
                 $$->operands[1] = $3;
                 }
                 | BR lb_operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 }
                 | op_condbr reg_operand lb_operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 $$->operands[1] = $3;
                 }
                 | CALL lb_operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 }
                 | LOAD reg_operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 }
                 | STORE operand reg_operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 $$->operands[0] = $3;
                 }
                 | READ { $$ = new Instruction(0, $1); }
                 | WRITE operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 }
                 | WRL { $$ = new Instruction(0, $1); }
                 | PARAM operand {
                 $$ = new Instruction(0, $1);
                 $$->operands[0] = $2;
                 }
                 ;

op_arith:
        ADD 
        | SUB
        | MUL
        | DIV
        | MOD
        | NEG
        | CMPEQ
        | CMPLE
        | CMPLT

op_condbr:
     BLBS | BLBC

operand:
       reg_operand { $$=$1; }
       | offset_exp { $$=$1; }
       | integer { 
       $$ = new Operand;
       $$->operand_type = OPERAND_CONST;
       $$->v = $1;
       }
       ;

reg_operand: 
       REG { 
       $$ = new Operand; 
       $$->operand_type = OPERAND_REG; 
       $$->v = atoi($1->substr(1,$1->size()-2).c_str());
       }

lb_operand:
          INS_LB {
          $$ = new Operand;
          $$->operand_type = OPERAND_LABEL;
          $$->v = atoi($1->substr(1,$1->size()-2).c_str());
          }

offset_exp:
          IDENTIFIER SHARP integer { 
          $$ = new Operand;
          $$->operand_type = OPERAND_OFFSET;
          $$->name = *($1);
          $$->v = $3;
          }
          ;

integer: INTEGER { $$=atoi($1->c_str()); delete $1; }
