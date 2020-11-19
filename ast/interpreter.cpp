#include <ast/ast.h>

int interpretExprAst(AstNodeC* const astNode)
{
  int leftValue, rightValue;
  
  if (astNode->left)
    leftValue = interpretExprAst(astNode->left);
  if (astNode->right)
    rightValue = interpretExprAst(astNode->right);
    
  switch(astNode->type)
  {
    case AST_ADD:
      return leftValue + rightValue;
    case AST_SUBTRACT:
      return leftValue - rightValue;
    case AST_MULTIPLY:
      return leftValue * rightValue;
    case AST_DIVIDE:
      return leftValue / rightValue;
    case AST_INTEGER:
      return astNode->intValue;
  }
}