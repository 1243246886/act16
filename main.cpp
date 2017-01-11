#include "ast-nodes.hpp"
#include "parser.hpp"
#include <iostream>

using namespace std;

extern Program *myProgram;

int main()
{
    yyparse();

    cout << "Program entry is instruction " << myProgram->entry << endl;
    for (int i=0; i<myProgram->flist.size(); i++) {
        cout << "Function " << i+1 << ": entry is " << 
            myProgram->flist[i]->entry << endl;
    }
    return 0;
}

