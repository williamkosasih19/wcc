#include <components/IoC.h>

#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <sstream>
#include <deque>

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
    exit -1;
  }
  
  stringstream sstream;
  sstream << inFile.rdbuf();

  const string& sstreamStr = sstream.str();
  buffer = deque<char>(sstreamStr.cbegin(), sstreamStr.cend());

  inFile.close();
}

char IoC::advance()
{
  const char nextChar = buffer.front();
  buffer.pop_front();
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

void IoC::skip()
{
  if (buffer.size())
    buffer.pop_front();
}

bool IoC::empty()
{
  return buffer.empty();
}