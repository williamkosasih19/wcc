#pragma once

#include <components/TokenC.h>
#include <vector>
#include <memory>

enum astTypeE
{
  AST_EXPRESSION,
  AST_STATEMENT,
  AST_STATEMENTS
};

enum ast_expressionTypeE
{
  AST_EXPRESSION_ADD,
  AST_EXPRESSION_SUBTRACT,
  AST_EXPRESSION_MULTIPLY,
  AST_EXPRESSION_DIVIDE,
  AST_EXPRESSION_INTEGER
};

enum ast_statementTypeE
{
  AST_STATEMENT_PRINT,
  AST_STATEMENT_DECLARATION,
  AST_STATEMENT_ASSIGNMENT
};

class AstNodeC
{
  public:
  astTypeE type;
  std::shared_ptr<AstNodeC> left;
  std::shared_ptr<AstNodeC> right;
  int intValue;
  
  std::vector<std::shared_ptr<AstNodeC>> statements;
  
  ast_statementTypeE statementType;
  std::shared_ptr<AstNodeC> statementChild;
  
  ast_expressionTypeE expressionType;
  
};

std::shared_ptr<AstNodeC> createAstNode(const astTypeE type, const std::shared_ptr<AstNodeC> left,
                        const std::shared_ptr<AstNodeC> right, const int intValue);

std::shared_ptr<AstNodeC> createAstLeaf(const astTypeE type, const int intValue);

std::shared_ptr<AstNodeC> createAstUnary(const astTypeE type, const std::shared_ptr<AstNodeC> left,
                       const int intValue);

int interpretExprAst(const std::shared_ptr<AstNodeC> astNode);

std::shared_ptr<AstNodeC> parse(const std::vector<TokenC>& tokenVector);