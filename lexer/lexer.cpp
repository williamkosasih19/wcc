#include <components/TokenC.h>
#include <components/IoC.h>

#include <vector>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

static IoC io;

static TokenC tokenize_star()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();
  
  io.skip();
  if (io.peek() == '=')
  {
    
  }
  else if(io.peek() == '/')
  {
    
  }
  return TokenC(TKN_ARITH_OP, "*", startLine, startColumn, TKN_STAR);
}

static TokenC tokenize_slash()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();
  
  io.skip();
  
  if (io.peek() == '=')
  {
    
  }
  else if(io.peek() == '/')
  {
    
  }
  return TokenC(TKN_ARITH_OP, "/", startLine, startColumn, TKN_SLASH);
}

static TokenC tokenize_plus()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();
  
  io.skip();
  
  if (io.peek() == '+')
  {
    
  }
  else if(io.peek() == '=')
  {
    
  }
  return TokenC(TKN_ARITH_OP, "+", startLine, startColumn, TKN_PLUS);
}

static TokenC tokenize_minus()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();
  
  io.skip();
  
  if (io.peek() == '-')
  {
    
  }
  else if(io.peek() == '=')
  {
    
  }
  return TokenC(TKN_ARITH_OP, "-", startLine, startColumn, TKN_MINUS);
}

static TokenC tokenize_equals()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();
  
  io.skip();
  if (io.peek() == '=')
  {
    
  }
  return TokenC(TKN_EQUALS, "=", startLine, startColumn, TKN_NO_SECOND_TYPE);
}

static TokenC tokenize_singleCharacterSymbols()
{
  const uint64_t startLine = io.getLine();
  const uint64_t startColumn = io.getColumn();

  TokenTypesE tokenType;
  
  char symbol;

  switch (symbol = (io.advance()))
  {
    case '{':
      tokenType = TKN_OPEN_CURLY_BRACKET;
      break;
    case '}':
      tokenType = TKN_CLOSE_CURLY_BRACKET;
      break;
    case '(':
      tokenType = TKN_OPEN_PARENTHESIS;
      break;
    case ')':
      tokenType = TKN_CLOSE_PARENTHESIS;
      break;
    case ';':
      tokenType = TKN_SEMICOLON;
      break;
  }
  return TokenC(tokenType, "" + symbol, startLine, startColumn);
}

static bool isKeyword(string str)
{
  if (str == "int" || str == "return" || str == "print")
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
    tokenType = TKN_KEYWORD;
  else 
    tokenType = TKN_IDENTIFIER;

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
  
  return TokenC(TKN_INTEGER, current_numbers,
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
      case '+':
        tokenVector.push_back(tokenize_plus());
        break;
      case '-':
        tokenVector.push_back(tokenize_minus());
        break;
      case '*':
        tokenVector.push_back(tokenize_star());
        break;
      case '/':
        tokenVector.push_back(tokenize_slash());
        break;
      case '=':
        tokenVector.push_back(tokenize_equals());
        break;
      default:
        io.skip();
        break;
    }
  }
  tokenVector.push_back(TokenC(TKN_EOF, -1, -1));
  
  return tokenVector;
}
