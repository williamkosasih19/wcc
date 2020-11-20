#pragma once

#include <components/TokenC.h>

#include <iostream>
#include <string>


void fatal(const std::string message,
           const TokenC token);

void fatal(const std::string message,
           const TokenC token,
           const TokenTypesE expectedTokenType);