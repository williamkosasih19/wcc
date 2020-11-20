#include <ast/ast.h>
#include <codegen/codegen.h>

#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <cstdarg>

using namespace std;

static vector<string> codegenOut;

class RegisterC
{
  private:
    vector<string> registerNames = {"r8", "r9", "r10", "r11"};
    vector<bool> freeRegisters;
    
    uint32_t numberOfRegisters;
  public:
    RegisterC()
    {
      numberOfRegisters = registerNames.size();
      freeRegisters.resize(numberOfRegisters);
      fill(freeRegisters.begin(), freeRegisters.end(), true);
    }
    
    void freeAll()
    {
      fill(freeRegisters.begin(), freeRegisters.end(), true);
    }
    
    uint32_t getRegister()
    {
      for (uint32_t i = 0; i < numberOfRegisters; i++)
      {
        if (freeRegisters[i])
        {
          freeRegisters[i] = false;
          return i;
        }
      }
      cerr << "Out of Free registers!" << endl;
      exit(-1); 
    }
    
    string getRegisterName(const uint32_t registerNumber) const
    {
      return registerNames[registerNumber];
    }
    
    void freeRegister(const uint32_t registerNumber)
    {
      freeRegisters[registerNumber] = true;
      return;
    }
} registers;

static string constructInstruction(const string& instr)
{
  return "\t" + instr;
}

static string constructInstruction(const string& instr,
                                   const string& a)
{
  return "\t" + instr + "\t" + a;
}

static string constructInstruction(const string& instr,
                                   const string& a,
                                   const string& b)
{
  return "\t" + instr + "\t" + a + "," + b;
}


static int codegen_load(const uint32_t value)
{
  const uint32_t regNumber = registers.getRegister();
  codegenOut.push_back(constructInstruction("movq", 
                                            "$" + to_string(value), 
                                            "%" + registers.getRegisterName(regNumber)));
  return regNumber;
}

static int codegen_add(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  codegenOut.push_back(constructInstruction("addq",
                                            "%" + registers.getRegisterName(leftRegisterNumber),
                                            "%" + registers.getRegisterName(rightRegisterNumber)));
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}

static int codegen_subtract(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  codegenOut.push_back(constructInstruction("subq",
                                            "%" + registers.getRegisterName(rightRegisterNumber),
                                            "%" + registers.getRegisterName(leftRegisterNumber)));
  registers.freeRegister(rightRegisterNumber);
  
  return leftRegisterNumber;
}

static int codegen_multiply(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  codegenOut.push_back(constructInstruction("imulq",
                                            "%" + registers.getRegisterName(leftRegisterNumber),
                                            "%" + registers.getRegisterName(rightRegisterNumber)));
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}

static int codegen_divide(const uint32_t leftRegisterNumber,
                          const uint32_t rightRegisterNumber)
{
  codegenOut.push_back(constructInstruction("movq", 
                                             "%" + registers.getRegisterName(leftRegisterNumber), "%rax"));
  codegenOut.push_back(constructInstruction("cqo"));
  codegenOut.push_back(constructInstruction("idivq", "%" + registers.getRegisterName(rightRegisterNumber)));
  codegenOut.push_back(constructInstruction("movq", "%rax", "%" + registers.getRegisterName(rightRegisterNumber)));
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}
                       
void codegen_printRegister(const uint32_t registerNumber)
{
  codegenOut.push_back(constructInstruction("movq", "%" + registers.getRegisterName(registerNumber),
                       "%rdi"));
  codegenOut.push_back(constructInstruction("call", "DEBUG_PRINTINT"));
}

void codegen_setup()
{
  registers.freeAll();
  codegenOut.push_back(constructInstruction(".text"));
  codegenOut.push_back(".PRINTINTFORMAT:");
  codegenOut.push_back(constructInstruction(".string", "\"The result is: %d\\n\""));
  
  codegenOut.push_back("DEBUG_PRINTINT:");
  codegenOut.push_back(constructInstruction("pushq", "%rbp"));
  codegenOut.push_back(constructInstruction("movq", "%rsp", "%rbp"));
  codegenOut.push_back(constructInstruction("subq", "$16", "%rsp"));
  codegenOut.push_back(constructInstruction("movq", "%rdi", "-8(%rbp)"));
  codegenOut.push_back(constructInstruction("movq", "%rdi", "%rax"));
  codegenOut.push_back(constructInstruction("movq", "%rdi", "%rsi"));
  codegenOut.push_back(constructInstruction("leaq", ".PRINTINTFORMAT(%rip)", "%rdi"));
  codegenOut.push_back(constructInstruction("movl", "$0", "%eax"));
  codegenOut.push_back(constructInstruction("call", "printf@PLT"));
  codegenOut.push_back(constructInstruction("leave"));
  codegenOut.push_back(constructInstruction("ret"));
  
  codegenOut.push_back(constructInstruction(".globl", "main"));
  codegenOut.push_back(constructInstruction(".type", "main", "@function"));
  
  codegenOut.push_back("main:");
  codegenOut.push_back(constructInstruction("pushq", "%rbp"));
  codegenOut.push_back(constructInstruction("movq", "%rsp", "%rbp"));
  
}
                       
void codegen_exit()
{
  codegenOut.push_back(constructInstruction("movq", "$0", "%rax"));
  codegenOut.push_back(constructInstruction("popq", "%rbp"));
  codegenOut.push_back(constructInstruction("ret"));
}

//

static int codegen_expr(const shared_ptr<AstNodeC> astNode)
{
  int leftReg, rightReg;
  
  if (astNode->left)
    leftReg = codegen_expr(astNode->left);
  if (astNode->right)
    rightReg = codegen_expr(astNode->right);
    
  switch (astNode->expressionType)
  {
    case AST_EXPRESSION_ADD:
      return codegen_add(leftReg, rightReg);
    case AST_EXPRESSION_SUBTRACT:
      return codegen_subtract(leftReg, rightReg);
    case AST_EXPRESSION_MULTIPLY:
      return codegen_multiply(leftReg, rightReg);
    case AST_EXPRESSION_DIVIDE:
      return codegen_divide(leftReg, rightReg);
    case AST_EXPRESSION_INTEGER:
      return codegen_load(astNode->intValue);
  }  
}

static void codegen_statement(const shared_ptr<AstNodeC> astNode)
{
  const uint32_t targetReg = codegen_expr(astNode->statementChild);
  if (astNode->statementType == AST_STATEMENT_PRINT)
    codegen_printRegister(targetReg);
  registers.freeRegister(targetReg);
  
  return;
}

static void codegen_statements(const shared_ptr<AstNodeC> astNode)
{
  for (const shared_ptr<AstNodeC> childAstNode : astNode->statements)
  {
    codegen_statement(childAstNode);
  }
  return;
}

vector<string> codegen(const shared_ptr<AstNodeC> astNode)
{
  codegen_setup();
  
  codegen_statements(astNode);
  
  codegen_exit();
  
  return codegenOut;
}