#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <deque>
#include <cstdint>

class IoC
{
  private:
  std::deque<char> buffer;

  uint64_t line, column;

  void checkIncrementLine();

  public:
  IoC(){};
  IoC(std::string filePath);

  char advance();
  char peek();
  char peekNext();
  void skip();
  bool empty();

  uint64_t getLine() const;
  uint64_t getColumn() const;
};