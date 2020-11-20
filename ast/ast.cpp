#include <ast/ast.h>

#include <memory>

using namespace std;

shared_ptr<AstNodeC> createAstNode(const astTypeE type, const shared_ptr<AstNodeC> left,
                        const shared_ptr<AstNodeC> right, const int intValue)
{
  shared_ptr<AstNodeC> astNodePtr = make_shared<AstNodeC>();
  
  astNodePtr->type = type;
  astNodePtr->left = left;
  astNodePtr->right = right;
  astNodePtr->intValue = intValue;
  
  return astNodePtr;
}

shared_ptr<AstNodeC> createAstLeaf(const astTypeE type, const int intValue)
{
  return createAstNode(type, nullptr, nullptr, intValue);
}

shared_ptr<AstNodeC> createAstUnary(const astTypeE type, const shared_ptr<AstNodeC> left,
                       const int intValue)
{
  return createAstNode(type, left, nullptr, intValue);
}