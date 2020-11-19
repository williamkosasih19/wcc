#include <lexer.h>
#include <components/TokenC.h>
#include <components/TokenTypesE.h>

#include <ast/ast.h>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
  vector<TokenC> tokenVector = lex(argv[1]);
  AstNodeC* astNode = parse(tokenVector);
  
  cout << interpretExprAst(astNode) << endl;
}