#include <components/TokenC.h>
#include <components/TokenTypesE.h>

#include <string>

using namespace std;

TokenC::TokenC(const TokenTypesE tokenType, const uint64_t line, const uint64_t column)
{
  this->tokenType = tokenType;
  this->line = line;
  this->column = column;
}

TokenC::TokenC(const TokenTypesE tokenType, const string& value, const uint64_t line, const uint64_t column)
{
  this->tokenType = tokenType;
  this->value = value;
  this->line = line;
  this->column = column;
}