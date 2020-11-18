#pragma once

#include <components/TokenC.h>
#include <vector>

enum astTypeE
{
  AST_ADD,
  AST_SUBTRACT,
  AST_MULTIPLY,
  AST_DIVIDE,
  AST_INTEGER
};

class AstNodeC
{
  public:
  astTypeE type;
  AstNodeC* left;
  AstNodeC* right;
  int intValue;
};

AstNodeC* createAstNode(const astTypeE type, AstNodeC* const left,
                        AstNodeC* const right, const int intValue);

AstNodeC* createAstLeaf(const astTypeE type, const int intValue);

AstNodeC* createAstUnary(const astTypeE type, AstNodeC* const left,
                       const int intValue);

int interpretExprAst(AstNodeC* const astNode);

AstNodeC* parse(const std::vector<TokenC>& tokenVector);