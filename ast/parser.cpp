#include <ast/ast.h>
#include <components/TokenC.h>
#include <components/TokenHandlerC.h>

#include <vector>

#include <iostream>

using namespace std;

static TokenHandlerC tokenHandler;

static astTypeE getArithAstTypeFromToken(const TokenC token)
{
  switch(token.secondTokenType)
  {
    case TKN_PLUS:
      return AST_ADD;
    case TKN_MINUS:
      return AST_SUBTRACT;
    case TKN_STAR:
      return AST_MULTIPLY;
    case TKN_SLASH:
      return AST_DIVIDE;
  }
  cerr << "BAD ARITH OPERATION TOKEN " << endl;
  cerr << "at " << token.line << ":" << token.column << endl;
  cerr << "[ " << token.value << " ]" << endl;
  exit(-1);
}

static AstNodeC* parse_expr()
{
  AstNodeC* left;
  
  const TokenC leftToken = tokenHandler.tokenMustBe(TKN_INTEGER);
  
  left = createAstLeaf(AST_INTEGER, stoi(leftToken.value));

  if (tokenHandler.empty())
    return left;
    
  const TokenC midToken = tokenHandler.tokenMustBe(TKN_ARITH_OP);
  
  const astTypeE astType = getArithAstTypeFromToken(midToken);
  
  AstNodeC* right = parse_expr();
  
  return createAstNode(astType, left, right, 0);
}

AstNodeC* parse(const vector<TokenC>& tokenVector)
{
  tokenHandler = TokenHandlerC(tokenVector);
  
  return parse_expr();
  

  
}