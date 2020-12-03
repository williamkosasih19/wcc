#include <components/TokenHandlerC.h>
#include <components/TokenC.h>
#include <common.h>

#include <deque>
#include <vector>

#include <iostream>

using namespace std;

TokenHandlerC::TokenHandlerC(const vector<TokenC>& tokenVector)
{
  tokens = deque<TokenC>(tokenVector.begin(), tokenVector.end());
  return;
}

TokenC TokenHandlerC::peekToken(const uint64_t index) const
{
  if (index < tokens.size())
    return tokens.at(index);
}

TokenC TokenHandlerC::advanceToken()
{
  if (tokens.size())
  {
    const TokenC currentToken = tokens.front();
    tokens.pop_front();
    return currentToken;
  }
}

void TokenHandlerC::skipToken()
{
  if (tokens.size())
    tokens.pop_front();
}

TokenC TokenHandlerC::tokenMustBe(TokenTypesE tokenType)
{
  const TokenC currentToken = tokens.front(); 
  if (currentToken.tokenType != tokenType)
  {    
    fatal("FATAL ERROR", currentToken, tokenType);
  }
  tokens.pop_front();
  return currentToken;
}
  
bool TokenHandlerC::empty() const
{
  return tokens.empty();
}

size_t TokenHandlerC::size() const 
{
  return tokens.size();
}