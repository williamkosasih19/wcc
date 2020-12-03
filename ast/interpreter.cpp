#include <ast/ast.h>

#include <memory>

using namespace std;

int interpretExprAst(const shared_ptr<AstNodeC> astNode)
{
  int leftValue, rightValue;
  
  if (astNode->expresstion_left)
    leftValue = interpretExprAst(astNode->expresstion_left);
  if (astNode->expression_right)
    rightValue = interpretExprAst(astNode->expression_right);
    
  switch(astNode->expr_infixOpType)
  {
    case AST_INFIX_ADD:
      return leftValue + rightValue;
    case AST_INFIX_SUB:
      return leftValue - rightValue;
    case AST_INFIX_MULTIPLY:
      return leftValue * rightValue;
    case AST_INFIX_DIVIDE:
      return leftValue / rightValue;
  }
}