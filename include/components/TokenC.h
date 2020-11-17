#pragma once

#include "TokenTypesE.h"
#include <string>

class TokenC
{
  public:
  TokenTypesE tokenType;
  std::string value;

  TokenC(TokenTypesE tokenType);
  TokenC(TokenTypesE tokenType, std::string value);
};