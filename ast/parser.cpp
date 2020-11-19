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

static AstNodeC* parse_multiplicativeExpr()
{
  AstNodeC* left;
  
  const TokenC leftToken = tokenHandler.tokenMustBe(TKN_INTEGER);
  left = createAstLeaf(AST_INTEGER, stoi(leftToken.value));
  
  if (tokenHandler.empty())
    return left;
  
  while (tokenHandler.peekToken().secondTokenType == TKN_STAR ||
         tokenHandler.peekToken().secondTokenType == TKN_SLASH)
  {
    const TokenC midToken = tokenHandler.advanceToken();
    
    AstNodeC* right;
    const TokenC rightToken = tokenHandler.tokenMustBe(TKN_INTEGER);
    right = createAstLeaf(AST_INTEGER, stoi(rightToken.value));
    
    left = createAstNode(getArithAstTypeFromToken(midToken), left, right, 0);
    
    if (tokenHandler.empty())
      break;
  }
  return left;
}

static AstNodeC* parse_additiveExpr()
{
  AstNodeC* left;
  left = parse_multiplicativeExpr();
  
  if (tokenHandler.empty())
    return left;
    
  while (true)
  {
    const TokenC midToken = tokenHandler.tokenMustBe(TKN_ARITH_OP);
    
    AstNodeC* right = parse_multiplicativeExpr();
    left = createAstNode(getArithAstTypeFromToken(midToken), left, right, 0);
    
    if (tokenHandler.empty())
      break;
  }
  
  return left;
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
  
  return parse_additiveExpr();
}