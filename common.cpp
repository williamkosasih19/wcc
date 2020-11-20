#include <common.h>
#include <components/TokenC.h>

#include <iostream>
#include <string>

using namespace std;


void fatal(const string message, const TokenC token)
{
  cerr << message << endl;
  cerr << "at " << token.line << ":" << token.column << endl;
  cerr << "[ " << token.value << " ]" << endl;
  exit (-1);
}

void fatal(const std::string message,
           const TokenC token,
           const TokenTypesE expectedTokenType)
{
    cerr << message + ": unexpected tokenType : " << getTokenRepresentation(token) << 
    " - expected: " << getTokenTypeRepresentation(expectedTokenType) << endl;
    cerr << "at " << token.line << ":" << token.column << endl;
    cerr << "[ " << token.value << " ]" << endl;
    exit (-1);
}