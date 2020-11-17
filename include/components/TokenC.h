#pragma once

#include "TokenTypesE.h"

#include <string>
#include <cstdint>

class TokenC
{
  public:
  TokenTypesE tokenType;
  std::string value = "";
  uint64_t line, column;

  TokenC(const TokenTypesE tokenType, const uint64_t line, const uint64_t column);
  TokenC(const TokenTypesE tokenType, const std::string& value, const uint64_t line, const uint64_t column);
};