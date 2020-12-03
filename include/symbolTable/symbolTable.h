#pragma once

#include <map>
#include <limits>
#include <string>

#include <cstdint>

const uint64_t 
  SYMBOLTABLE_DECLARED_BEFORE = std::numeric_limits<uint64_t>::max();

class globalSymbolTableC
{
  private:
    std::map<std::string, uint64_t> identifierSizeMap;
  public:
    bool isDeclared(const std::string& identifier);
    bool declare(const std::string& identifier, const uint64_t size);
    uint64_t getSize(const std::string& identifier);
    
    globalSymbolTableC();
};

class localSymbolTable_offsetSizeC
{
  public:
  uint64_t offset;
  uint64_t size;
  
  localSymbolTable_offsetSizeC(const uint64_t offset, const uint64_t size)
  {
    this->offset = offset;
    this->size = size;
  }
};

class localSymbolTableC
{
  private:
    std::map<std::string, localSymbolTable_offsetSizeC> identifierDataMap;
    uint64_t currentStackOffset;
  public:
    bool isDeclared(const std::string& identifier);
    bool declare(const std::string& identifier, const uint64_t size);
    localSymbolTable_offsetSizeC getVar();
    
    localSymbolTableC();
};