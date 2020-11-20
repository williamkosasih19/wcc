#include <ast/ast.h>

#include <memory>

using namespace std;

int interpretExprAst(const shared_ptr<AstNodeC> astNode)
{
  int leftValue, rightValue;
  
  if (astNode->left)
    leftValue = interpretExprAst(astNode->left);
  if (astNode->right)
    rightValue = interpretExprAst(astNode->right);
    
  switch(astNode->expressionType)
  {
    case AST_EXPRESSION_ADD:
      return leftValue + rightValue;
    case AST_EXPRESSION_SUBTRACT:
      return leftValue - rightValue;
    case AST_EXPRESSION_MULTIPLY:
      return leftValue * rightValue;
    case AST_EXPRESSION_DIVIDE:
      return leftValue / rightValue;
    case AST_EXPRESSION_INTEGER:
      return astNode->intValue;
  }
}