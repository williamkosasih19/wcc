#include <common.h>

#include <ast/ast.h>
#include <components/TokenC.h>
#include <components/TokenHandlerC.h>

#include <vector>

#include <iostream>

using namespace std;

static TokenHandlerC tokenHandler;

static ast_expressionTypeE getExpressionAstTypeFromToken(const TokenC token)
{
  switch(token.secondTokenType)
  {
    case TKN_PLUS:
      return AST_EXPRESSION_ADD;
    case TKN_MINUS:
      return AST_EXPRESSION_SUBTRACT;
    case TKN_STAR:
      return AST_EXPRESSION_MULTIPLY;
    case TKN_SLASH:
      return AST_EXPRESSION_DIVIDE;
  }
  cerr << "BAD ARITH OPERATION TOKEN " << endl;
  cerr << "at " << token.line << ":" << token.column << endl;
  cerr << "[ " << token.value << " ]" << endl;
  exit(-1);
}

static shared_ptr<AstNodeC> parse_multiplicativeExpr()
{
  shared_ptr<AstNodeC> left;
  
  const TokenC leftToken = tokenHandler.tokenMustBe(TKN_INTEGER);
  left = createAstLeaf(AST_EXPRESSION, stoi(leftToken.value));
  left->expressionType = AST_EXPRESSION_INTEGER;
  
  if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
    return left;
  
  while (tokenHandler.peekToken().secondTokenType == TKN_STAR ||
         tokenHandler.peekToken().secondTokenType == TKN_SLASH)
  {
    const TokenC midToken = tokenHandler.advanceToken();
    
    shared_ptr<AstNodeC> right;
    const TokenC rightToken = tokenHandler.tokenMustBe(TKN_INTEGER);
    right = createAstLeaf(AST_EXPRESSION, stoi(rightToken.value));
    right->expressionType = AST_EXPRESSION_INTEGER;
    
    left = createAstNode(AST_EXPRESSION, left, right, 0);
    left->expressionType = getExpressionAstTypeFromToken(midToken);
    
    if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
      break;
  }
  return left;
}

static shared_ptr<AstNodeC> parse_expr()
{
  shared_ptr<AstNodeC> left;
  left = parse_multiplicativeExpr();
  
  if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
    return left;
    
  while (true)
  {
    const TokenC midToken = tokenHandler.tokenMustBe(TKN_ARITH_OP);
    
    shared_ptr<AstNodeC> right = parse_multiplicativeExpr();
    left = createAstNode(AST_EXPRESSION, left, right, 0);
    left->expressionType = getExpressionAstTypeFromToken(midToken);
    
    if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
      break;
  }
  
  return left;
}

static shared_ptr<AstNodeC> parse_statement()
{
  const TokenC currentToken = tokenHandler.advanceToken();
  
  shared_ptr<AstNodeC> statementAst = make_shared<AstNodeC>();
  statementAst->type = AST_STATEMENT;
  
  if (currentToken.tokenType == TKN_KEYWORD)
  {
    if (currentToken.value == "print")
      statementAst->statementType = AST_STATEMENT_PRINT;
  }
  else
  {
    fatal("UNRECOGNIZED STATEMENT", currentToken);
  }
  
  statementAst->statementChild = parse_expr();
  
  tokenHandler.tokenMustBe(TKN_SEMICOLON);  
  return statementAst;
}

static shared_ptr<AstNodeC> parse_statements()
{
  shared_ptr<AstNodeC> statementsAst = make_shared<AstNodeC>();
  statementsAst->type = AST_STATEMENTS;
  
  while (tokenHandler.peekToken().tokenType != TKN_EOF)
  {
    statementsAst->statements.push_back(parse_statement());
  }
  
  return statementsAst;
}

shared_ptr<AstNodeC> parse(const vector<TokenC>& tokenVector)
{
  tokenHandler = TokenHandlerC(tokenVector);
  
  return parse_statements();
}