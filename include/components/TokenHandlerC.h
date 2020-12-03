#pragma once

#include "TokenC.h"

#include <vector>
#include <deque>

class TokenHandlerC
{
  private:
  std::deque<TokenC> tokens;
  
  public:
  TokenHandlerC(){};
  TokenHandlerC(const std::vector<TokenC>& tokenVector);
  TokenC peekToken(const uint64_t index = 0) const;
  TokenC advanceToken();
  void skipToken();
  
  TokenC tokenMustBe(TokenTypesE tokenType);  
  
  bool empty() const;
  
  size_t size() const;
};