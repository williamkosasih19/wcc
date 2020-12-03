#include <common.h>
#include <symbolTable/symbolTable.h>

#include <string>
#include <cstdint>

using namespace std;

globalSymbolTableC::globalSymbolTableC()
{
  
}
bool globalSymbolTableC::isDeclared(const std::string& identifier)
{
  return (identifierSizeMap.find(identifier) != identifierSizeMap.end());
}
bool globalSymbolTableC::declare(const std::string& identifier, const uint64_t size)
{
  if (isDeclared(identifier))
    return false;
  identifierSizeMap[identifier] = size;
  return true;
}
uint64_t globalSymbolTableC::getSize(const std::string& identifier)
{
  return identifierSizeMap[identifier];
}