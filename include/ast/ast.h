#pragma once

#include <components/TokenC.h>
#include <vector>
#include <memory>

enum astTypeE
{
  AST_EXPRESSION,
  AST_STATEMENT,
  AST_STATEMENTS,
  AST_TERM,
  AST_VAR_TERM,
  AST_INDEX,
  AST_EXPR_LIST,
  AST_INFIX_OP,
  AST_UNARY_OP
};


// expr             ::= term (infix_op term)*
// term             ::= integer_constant | string_constant | 'true' | 'false' | 'null' | 'this' | '(' expr ')' | unary_op term | var_term
// var_term         ::= identifier (index | id_call | call)?
// index            ::= '[' expr ']'
// id_call          ::= '.' identifier call
// call             ::= '(' expr_list ')'
// expr_list        ::= (expr (',' expr)*)?
// infix_op         ::= '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
// unary_op         ::= '-' | '~'

// enum ast_expressionTypeE
// {
//   AST_EXPRESSION_ADD,
//   AST_EXPRESSION_SUBTRACT,
//   AST_EXPRESSION_MULTIPLY,
//   AST_EXPRESSION_DIVIDE,
//   AST_EXPRESSION_INTEGER
// };

enum ast_termTypeE
{
  AST_TERM_INTEGER_CONSTANT,
  AST_TERM_STRING_CONSTANT,
  AST_TERM_BOOL_CONSTANT,
  AST_TERM_NULL,
  AST_TERM_THIS,
  AST_TERM_EXPRESSION_LIST,
  AST_TERM_UNARY_OP,
  AST_TERM_VAR
};

enum ast_varTermTypeE
{
  AST_VAR_TERM_PLAIN,
  AST_VAR_TERM_WITH_INDEX,
  AST_VAR_TERM_FUNCTION_CALL
};

enum ast_varTermSegmentE
{
  AST_VAR_TERM_GLOBAL,
  AST_VAR_TERM_LOCAL
};

enum ast_infixOpTypeE
{
  AST_INFIX_ADD,
  AST_INFIX_SUB,
  AST_INFIX_MULTIPLY,
  AST_INFIX_DIVIDE,
  AST_INFIX_BITWISE_AND,
  AST_INFIX_BITWISE_OR,
  AST_INFIX_LESS_THAN,
  AST_INFIX_GREATER_THAN,
  AST_INFIX_EQUALS,
  AST_INFIX_NOT_EQUALS,
  AST_INFIX_LESS_THAN_EQUALS,
  AST_INFIX_GREATER_THAN_EQUALS
};

enum ast_unaryOpTypeE
{
  AST_UNARY_NEGATIVE,
  AST_UNARY_BITWISE_NOT
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
  
  std::vector<std::shared_ptr<AstNodeC>> statements;
  
  ast_statementTypeE statement_statementType;
  std::shared_ptr<AstNodeC> statement_statementChild;
  
  ast_infixOpTypeE expr_infixOpType;
  
  std::shared_ptr<AstNodeC> expresstion_left;
  std::shared_ptr<AstNodeC> expression_right;
  
  ast_termTypeE term_termType;
  
  int64_t integerConstant_intValue;
  
  bool boolConstant_boolValue;
  
  std::vector<std::shared_ptr<AstNodeC>> expressionList_expressionList;
  
  ast_unaryOpTypeE unaryOp_unaryOpType;
  
  std::string stringConstant_stringValue;
  
  ast_varTermTypeE varTerm_varTermType;
  ast_varTermSegmentE varTerm_varTermSegment;
  
  std::string varTerm_globalVariable_variableName;
  uint64_t varTerm_globalVariable_variableSize;
};

std::shared_ptr<AstNodeC> createAstNode(const astTypeE type, const std::shared_ptr<AstNodeC> left,
                        const std::shared_ptr<AstNodeC> right, const int intValue);

std::shared_ptr<AstNodeC> createAstLeaf(const astTypeE type, const int intValue);

std::shared_ptr<AstNodeC> createAstUnary(const astTypeE type, const std::shared_ptr<AstNodeC> left,
                       const int intValue);

int interpretExprAst(const std::shared_ptr<AstNodeC> astNode);

std::shared_ptr<AstNodeC> parse(const std::vector<TokenC>& tokenVector);