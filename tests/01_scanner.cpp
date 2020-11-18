#include <lexer.h>
#include <components/TokenC.h>
#include <components/TokenTypesE.h>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
  vector<TokenC> tokenVector = lex(argv[1]);
  for (const TokenC token : tokenVector)
  {
    cout << "Token " << getTokenRepresentation(token);
    if (token.tokenType == TKN_INTEGER)
      cout << ", value " << token.value;
    cout << endl;
  }
}