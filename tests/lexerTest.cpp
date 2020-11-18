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
    cout << "Type : ";
    switch (token.tokenType)
    {
      case TKN_OPEN_CURLY_BRACKET:
        cout << "{";
        break;
      case TKN_CLOSE_CURLY_BRACKET:
        cout << "}";
        break;
      case TKN_OPEN_PARENTHESIS:
        cout << "(";
        break;
      case TKN_CLOSE_PARENTHESIS:
        cout << ")";
        break;
      case TKN_SEMICOLON:
        cout << ";";
        break;
      case TKN_KEYWORD:
        cout << "KEYWORD";
        break;
      case TKN_IDENTIFIER:
        cout << "IDENTIFIER";
        break;
      case TKN_INTEGER:
        cout << "INTEGER";
        break;
    }
    cout << "\t Value : " << token.value << "\t" << 
          token.line << ":" << token.column << endl;
  }
}