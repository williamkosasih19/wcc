#include <components/IoC.h>

#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <sstream>
#include <deque>
#include <cstdint>

using namespace std;

IoC::IoC(string filePath)
{
  fstream inFile;
  try
  {
    inFile.open(filePath);
  }
  catch(const std::exception& e)
  {
    cerr << "Invalid input file!" << endl;
    exit(-1);
  }
  
  stringstream sstream;
  sstream << inFile.rdbuf();

  const string& sstreamStr = sstream.str();
  buffer = deque<char>(sstreamStr.cbegin(), sstreamStr.cend());

  inFile.close();

  line = 1;
  column = 0;
}

void IoC::checkIncrementLine()
{
  if (peek() == '\n')
  {
    column = 1;
    line++;
  }
}

char IoC::advance()
{
  const char nextChar = buffer.front();
  buffer.pop_front();

  column++;
  checkIncrementLine();
  return nextChar;
}

char IoC::peek()
{
  if (buffer.size());
    return buffer.front();
  return EOF;
}

char IoC::peekNext()
{
  if (buffer.size() == 1)
    return EOF;
  return buffer[1];
}

void IoC::skip(const uint64_t ammount)
{
  for (uint64_t i = 0; i < ammount && i < buffer.size(); i++)
  {
    buffer.pop_front();
    column++;
  }
  checkIncrementLine();
}

bool IoC::empty()
{
  return buffer.empty();
}

uint64_t IoC::getLine() const
{
  return line;
}

uint64_t IoC::getColumn() const
{
  return column;
}