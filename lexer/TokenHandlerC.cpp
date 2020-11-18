#include <components/TokenHandlerC.h>
#include <components/TokenC.h>

#include <deque>
#include <vector>

#include <iostream>

using namespace std;

TokenHandlerC::TokenHandlerC(const vector<TokenC>& tokenVector)
{
  tokens = deque<TokenC>(tokenVector.begin(), tokenVector.end());
  return;
}

TokenC TokenHandlerC::peekToken() const
{
  if (tokens.size())
    return tokens.front();
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
    cerr << "unexpected tokenType : " << getTokenRepresentation(currentToken) << 
    " - expected: " << getTokenTypeRepresentation(tokenType) << endl;
    cerr << "at " << currentToken.line << ":" << currentToken.column << endl;
    cerr << "[ " << currentToken.value << " ]" << endl;
    exit (-1);
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