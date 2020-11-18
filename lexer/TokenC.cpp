#include <components/TokenC.h>
#include <components/TokenTypesE.h>

#include <string>

using namespace std;

TokenC::TokenC(const TokenTypesE tokenType, const uint64_t line, 
               const uint64_t column, const TokenTypesE secondTokenType)
{
  this->tokenType = tokenType;
  this->line = line;
  this->column = column;
  this->secondTokenType = secondTokenType;
}

TokenC::TokenC(const TokenTypesE tokenType, const string& value, const uint64_t line, const uint64_t column, const TokenTypesE secondTokenType)
{
  this->tokenType = tokenType;
  this->value = value;
  this->line = line;
  this->column = column;
  this->secondTokenType = secondTokenType;
}

string getTokenTypeRepresentation(const TokenTypesE tokenType)
{
  switch (tokenType)
  {
   case TKN_OPEN_CURLY_BRACKET:
        return "{";
      case TKN_CLOSE_CURLY_BRACKET:
        return "}";
      case TKN_OPEN_PARENTHESIS:
        return "(";
      case TKN_CLOSE_PARENTHESIS:
        return ")";
      case TKN_SEMICOLON:
        return ";";
      case TKN_KEYWORD:
        return "KEYWORD";
      case TKN_IDENTIFIER:
        return "IDENTIFIER";
      case TKN_INTEGER:
        return "INTEGER";
      case TKN_STAR:
        return "*";
      case TKN_SLASH:
        return "/";
      case TKN_PLUS:
        return "+";
      case TKN_MINUS:
        return "-";
      case TKN_ARITH_OP:
        return "ARITHMETIC_OPERATOR";
  }
}

string getTokenRepresentation(const TokenC token)
{
  return getTokenTypeRepresentation(token.tokenType);
}