#include <components/TokenC.h>
#include <components/TokenTypesE.h>

#include <string>

using namespace std;

TokenC::TokenC(TokenTypesE tokenType)
{
  this->tokenType = tokenType;
}

TokenC::TokenC(TokenTypesE tokenType, string value)
{
  this->tokenType = tokenType;
  this->value = value;
}