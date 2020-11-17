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
      case OPEN_CURLY_BRACKET:
        cout << "{";
        break;
      case CLOSE_CURLY_BRACKET:
        cout << "}";
        break;
      case OPEN_PARENTHESIS:
        cout << "(";
        break;
      case CLOSE_PARENTHESIS:
        cout << ")";
        break;
      case SEMICOLON:
        cout << ";";
        break;
      case KEYWORD:
        cout << "KEYWORD";
        break;
      case IDENTIFIER:
        cout << "IDENTIFIER";
        break;
      case INTEGER:
        cout << "INTEGER";
        break;
    }
    cout << "\t Value : " << token.value << "\t" << 
          token.line << ":" << token.column << endl;
  }
}