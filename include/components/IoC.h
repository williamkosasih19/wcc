#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <deque>

class IoC
{
  private:
  std::deque<char> buffer;

  public:
  IoC(std::string filePath);

  char advance();
  char peek();
  char peekNext();
  void skip();
  bool empty();
};