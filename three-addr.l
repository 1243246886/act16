%{
#include <string>
#include "ast-nodes.hpp"
#include "parser.hpp"

#define SAVE_TOKEN yylval.str=new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token=t)
%} 

%%

"instr" return TOKEN(INSTR);
"enter" return TOKEN(ENTER);
"ret" return TOKEN(RET);
"entrypc" return TOKEN(ENTRYPC);
[+-]?[0-9]+ SAVE_TOKEN; return INTEGER;
"add" return TOKEN(ADD);
"sub" return TOKEN(SUB);
"mul" return TOKEN(MUL);
"mod" return TOKEN(MOD);
"div" return TOKEN(DIV);
"cmpeq" return TOKEN(CMPEQ);
"cmple" return TOKEN(CMPLE);
"cmplt" return TOKEN(CMPLT);
"neg" return TOKEN(NEG);
"blbs" return TOKEN(BLBS);
"blbc" return TOKEN(BLBC);
\([0-9]+\) SAVE_TOKEN; return REG;
\[[0-9]+\] SAVE_TOKEN; return INS_LB;
[a-zA-Z0-9_]+ SAVE_TOKEN; return IDENTIFIER;
[\t\n ] {}
"#" return SHARP;
":" return COLON;

%%

