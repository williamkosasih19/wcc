#pragma once

#include <ast/ast.h>

#include <vector>
#include <string>

std::vector<std::string> codegen(const std::shared_ptr<AstNodeC> astNode);