#pragma once

#include "TokenTypesE.h"

#include <string>
#include <cstdint>

class TokenC
{
  public:
  TokenTypesE tokenType;
  TokenTypesE secondTokenType;
  std::string value = "";
  uint64_t line, column;

  TokenC(const TokenTypesE tokenType, const uint64_t line, const uint64_t column, const TokenTypesE secondTokenType = TKN_NO_SECOND_TYPE);
  TokenC(const TokenTypesE tokenType, const std::string& value, const uint64_t line, const uint64_t column, const TokenTypesE secondTokenType = TKN_NO_SECOND_TYPE);
};

std::string getTokenRepresentation(const TokenC token);
std::string getTokenTypeRepresentation(const TokenTypesE tokenType);