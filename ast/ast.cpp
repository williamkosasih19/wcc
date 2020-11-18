#include <ast/ast.h>

AstNodeC* createAstNode(const astTypeE type, AstNodeC* const left,
                        AstNodeC* const right, const int intValue)
{
  AstNodeC* astNodePtr = new AstNodeC();
  
  astNodePtr->type = type;
  astNodePtr->left = left;
  astNodePtr->right = right;
  astNodePtr->intValue = intValue;
  
  return astNodePtr;
}

AstNodeC* createAstLeaf(const astTypeE type, const int intValue)
{
  return createAstNode(type, nullptr, nullptr, intValue);
}

AstNodeC* createAstUnary(const astTypeE type, AstNodeC* const left,
                       const int intValue)
{
  return createAstNode(type, left, nullptr, intValue);
}