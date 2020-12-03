#include <common.h>

#include <ast/ast.h>
#include <components/TokenC.h>
#include <components/TokenHandlerC.h>
#include <symbolTable/symbolTable.h>

#include <vector>

#include <iostream>

using namespace std;

static TokenHandlerC tokenHandler;
static globalSymbolTableC globalSymbolTable;

static shared_ptr<AstNodeC> parse_expr();

static ast_infixOpTypeE getExpressionAstTypeFromToken(const TokenC token)
{
  switch(token.secondTokenType)
  {
    case TKN_PLUS:
      return AST_INFIX_ADD;
    case TKN_MINUS:
      return AST_INFIX_SUB;
    case TKN_STAR:
      return AST_INFIX_MULTIPLY;
    case TKN_SLASH:
      return AST_INFIX_DIVIDE;
  }
  cerr << "BAD ARITH OPERATION TOKEN " << endl;
  cerr << "at " << token.line << ":" << token.column << endl;
  cerr << "[ " << token.value << " ]" << endl;
  exit(-1);
}

static shared_ptr<AstNodeC> parse_term()
{
  const TokenC termToken = tokenHandler.advanceToken();
  
  shared_ptr<AstNodeC> term = make_shared<AstNodeC>();
  term->type = AST_TERM;
  
  if (termToken.tokenType == TKN_INTEGER)
  {
    term->term_termType = AST_TERM_INTEGER_CONSTANT;
    term->integerConstant_intValue = stoi(termToken.value);
    
    term->term_termType = AST_TERM_INTEGER_CONSTANT;
  }
  else if (termToken.tokenType == TKN_IDENTIFIER)
  {
    if (!globalSymbolTable.isDeclared(termToken.value))
      fatal("UNDECLARED VARIABLE", termToken);
    term->term_termType = AST_TERM_VAR;
    
    term->varTerm_varTermType = AST_VAR_TERM_PLAIN;
    term->varTerm_varTermSegment = AST_VAR_TERM_GLOBAL;
    
    term->varTerm_globalVariable_variableName = termToken.value;
    term->varTerm_globalVariable_variableSize = 
      globalSymbolTable.getSize(termToken.value);
  }
  else if(termToken.tokenType == TKN_OPEN_PARENTHESIS)
  {
    term = parse_expr();
    tokenHandler.tokenMustBe(TKN_CLOSE_PARENTHESIS);
  }
  else
  {
    fatal("INVALID TOKEN TO START EXPRESSION",
          termToken);
  }
  
  return term;
}

static shared_ptr<AstNodeC> parse_multiplicativeExpr()
{
  // shared_ptr<AstNodeC> left = make_shared<AstNodeC>();
  // left->type = AST_TERM;
  
  shared_ptr<AstNodeC> left = parse_term();  
  
  if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON ||
      tokenHandler.peekToken().tokenType == TKN_CLOSE_PARENTHESIS)
    return left;
  
  while (tokenHandler.peekToken().secondTokenType == TKN_STAR ||
         tokenHandler.peekToken().secondTokenType == TKN_SLASH)
  {
    const TokenC midToken = tokenHandler.advanceToken();
    
    shared_ptr<AstNodeC> right = parse_term();
    
    shared_ptr<AstNodeC> tempExpr = make_shared<AstNodeC>();
    tempExpr->type = AST_EXPRESSION;
    tempExpr->expresstion_left = left;
    tempExpr->expression_right = right;
    
    left = tempExpr;
    
    left->expr_infixOpType = getExpressionAstTypeFromToken(midToken);
    
    if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
      break;
  }
  return left;
}

static shared_ptr<AstNodeC> parse_expr()
{
  shared_ptr<AstNodeC> left;
  left = parse_multiplicativeExpr();
  
  if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON ||
      tokenHandler.peekToken().tokenType == TKN_CLOSE_PARENTHESIS)
    return left;
    
  while (true)
  {
    const TokenC midToken = tokenHandler.tokenMustBe(TKN_ARITH_OP);
    
    shared_ptr<AstNodeC> right = parse_multiplicativeExpr();
    
    shared_ptr<AstNodeC> tempExpr = make_shared<AstNodeC>();
    tempExpr->type = AST_EXPRESSION;
    tempExpr->expresstion_left = left;
    tempExpr->expression_right = right;
    
    left = tempExpr;
    left->expr_infixOpType = getExpressionAstTypeFromToken(midToken);
    
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
      statementAst->statement_statementType = AST_STATEMENT_PRINT;
    if (currentToken.value == "int")
    {
      const TokenC identifierToken = tokenHandler.tokenMustBe(TKN_IDENTIFIER);
      statementAst->statement_statementType = AST_STATEMENT_DECLARATION;
      
      statementAst->varTerm_varTermSegment = AST_VAR_TERM_GLOBAL;
      statementAst->varTerm_varTermType = AST_VAR_TERM_PLAIN;
      
      statementAst->varTerm_globalVariable_variableSize = 8;
      statementAst->varTerm_globalVariable_variableName = identifierToken.value;
      
      // Only declare 8 bytes integer value for now
      if (!globalSymbolTable.declare(identifierToken.value, 8))
        fatal("VARIABLE ALREADY DECLARED", identifierToken);  
    }
  }
  else if (currentToken.tokenType == TKN_IDENTIFIER)
  {
    if (!globalSymbolTable.isDeclared(currentToken.value))
      fatal("UNDECLARED VARIABLE", currentToken);
    
    const TokenC nextToken = tokenHandler.peekToken();
    
    switch (nextToken.tokenType)
    {
      case TKN_EQUALS:
        tokenHandler.skipToken();
        statementAst->type = AST_STATEMENT;
        statementAst->statement_statementType = AST_STATEMENT_ASSIGNMENT;
        
        statementAst->varTerm_varTermSegment = AST_VAR_TERM_GLOBAL;
        statementAst->varTerm_varTermType = AST_VAR_TERM_PLAIN;
        
        statementAst->varTerm_globalVariable_variableSize = globalSymbolTable.getSize(currentToken.value);
        statementAst->varTerm_globalVariable_variableName = currentToken.value;
        break;
    }
  }
  else
  {
    fatal("UNRECOGNIZED STATEMENT", currentToken);
  }
  if (tokenHandler.peekToken().tokenType == TKN_SEMICOLON)
    goto statementNoChild;
  
  statementAst->statement_statementChild = parse_expr();
  
  statementNoChild:
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