#include <components/TokenC.h>
#include <components/IoC.h>

#include <vector>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

static IoC io;

static TokenC tokenize_singleCharacterSymbols()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();

  TokenTypesE tokenType;

  switch (io.advance())
  {
    case '{':
      tokenType = OPEN_CURLY_BRACKET;
      break;
    case '}':
      tokenType = CLOSE_CURLY_BRACKET;
      break;
    case '(':
      tokenType = OPEN_PARENTHESIS;
      break;
    case ')':
      tokenType = CLOSE_PARENTHESIS;
      break;
    case ';':
      tokenType = SEMICOLON;
      break;
  }
  return TokenC(tokenType, startLine, startColumn);
}

static bool isKeyword(string str)
{
  if (str == "int" || str == "return")
    return true;
  return false;
}

static TokenC tokenize_characters()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();

  string currentCharacters = "";

  while(!io.empty())
  {
    switch (io.peek())
    {
      case '0' ... '9':
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '_':
        currentCharacters += io.advance();
        break;
      default:
        goto L_tokenize_charactersBreak;
        break;
    }
  }
  L_tokenize_charactersBreak:
  TokenTypesE tokenType;

  if (isKeyword(currentCharacters))
    tokenType = KEYWORD;
  else 
    tokenType = IDENTIFIER;

  return TokenC(tokenType, currentCharacters,
                startLine, startColumn);
}

static TokenC tokenize_integer()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();

  string current_numbers = "";
  while (io.peek() == '0')
    io.advance();

  while (isdigit(io.peek()))
    current_numbers += io.advance();
  
  return TokenC(INTEGER, current_numbers,
                startLine, startColumn);
}

vector<TokenC> lex(string filePath)
{
  io = IoC(filePath);

  vector<TokenC> tokenVector;
  
  while (!io.empty())
  {
    switch (io.peek())
    {
      case '{':
      case '}':
      case '(':
      case ')':
      case ';':
        tokenVector.push_back(tokenize_singleCharacterSymbols());
        break;
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '_':
        tokenVector.push_back(tokenize_characters());
        break;
      case '0' ... '9':
        tokenVector.push_back(tokenize_integer());
        break;
      default:
        io.skip();
        break;
    }
  }
  return tokenVector;
}