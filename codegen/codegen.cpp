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

#define EMIT1(x) codegenOut.push_back(constructInstruction(x));
#define EMIT2(x, y) codegenOut.push_back(constructInstruction(x, y));
#define EMIT3(x, y, z) codegenOut.push_back(constructInstruction(x, y, z));

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
  EMIT3("movq", "$" + to_string(value), 
        "%" + registers.getRegisterName(regNumber));
  return regNumber;
}

static int codegen_add(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  EMIT3("addq", "%" + registers.getRegisterName(leftRegisterNumber),
        "%" + registers.getRegisterName(rightRegisterNumber));
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}

static int codegen_subtract(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  EMIT3("subq", "%" + registers.getRegisterName(rightRegisterNumber),
        "%" + registers.getRegisterName(leftRegisterNumber));
  registers.freeRegister(rightRegisterNumber);
  
  return leftRegisterNumber;
}

static int codegen_multiply(const uint32_t leftRegisterNumber,
                       const uint32_t rightRegisterNumber)
{
  EMIT3("imulq", "%" + registers.getRegisterName(leftRegisterNumber),
        "%" + registers.getRegisterName(rightRegisterNumber));
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}

static int codegen_divide(const uint32_t leftRegisterNumber,
                          const uint32_t rightRegisterNumber)
{
  const string leftRegisterName = registers.getRegisterName(leftRegisterNumber);
  const string rightRegisterName = 
    registers.getRegisterName(rightRegisterNumber);
  EMIT3("movq", "%" + leftRegisterName, "%rax");
  EMIT1("cqo");
  EMIT2("idivq", "%" + rightRegisterName);
  EMIT3("movq", "%rax", "%" + rightRegisterName);
  registers.freeRegister(leftRegisterNumber);
  
  return rightRegisterNumber;
}


static int codegen_compare(const uint32_t leftRegister,
                           const uint32_t rightRegister,
                           const string& instruction)
{
  const string leftRegisterName = registers.getRegisterName(leftRegister);
  const string rightRegisterName = registers.getRegisterName(rightRegister);
  
  EMIT3("cmpq", "%" + leftRegisterName,
       "%" + rightRegisterName);
  EMIT2(instruction, "%" + leftRegisterName + "b");
  EMIT3("andq", "$255", "%" + leftRegisterName);
  registers.freeRegister(rightRegister);
  return leftRegister;
}
                       
void codegen_printRegister(const uint32_t registerNumber)
{
  EMIT3("movq", "%" + registers.getRegisterName(registerNumber),
                       "%rdi");
  EMIT2("call", "DEBUG_PRINTINT");
}

void codegen_setup()
{
  registers.freeAll();
  EMIT1(".text");
  codegenOut.push_back(".PRINTINTFORMAT:");
  EMIT2(".string", "\"The result is: %d\\n\"");
  
  codegenOut.push_back("DEBUG_PRINTINT:");
  EMIT2("pushq", "%rbp");
  EMIT3("movq", "%rsp", "%rbp");
  EMIT3("subq", "$16", "%rsp");
  EMIT3("movq", "%rdi", "-8(%rbp)");
  EMIT3("movq", "%rdi", "%rax");
  EMIT3("movq", "%rdi", "%rsi");
  EMIT3("leaq", ".PRINTINTFORMAT(%rip)", "%rdi");
  EMIT3("movl", "$0", "%eax");
  EMIT2("call", "printf@PLT");
  EMIT1("leave");
  EMIT1("ret");
  
  EMIT2(".globl", "main");
  EMIT3(".type", "main", "@function");
  
  codegenOut.push_back("main:");
  EMIT2("pushq", "%rbp");
  EMIT3("movq", "%rsp", "%rbp");
  
}
                       
void codegen_exit()
{
  EMIT3("movq", "$0", "%rax");
  EMIT2("popq", "%rbp");
  EMIT1("ret");
}

static int codegen_varTerm(const shared_ptr<AstNodeC>& astNode)
{
  int reg;
  switch (astNode->varTerm_varTermSegment)
  {
    case AST_VAR_TERM_GLOBAL:
      switch (astNode->varTerm_varTermType)
      {
        case AST_VAR_TERM_PLAIN:
        reg = registers.getRegister();
        
        if (astNode->varTerm_globalVariable_variableSize == 8)
          codegenOut.push_back(
            constructInstruction("movq", 
                                 astNode->varTerm_globalVariable_variableName + "(" + "%rip" + ")", 
                                 "%" + registers.getRegisterName(reg)));
        break;
      }
      break;
  }
  return reg;
}

static int codegen_term(const shared_ptr<AstNodeC>& astNode)
{
  switch (astNode->term_termType)
  {
    case AST_TERM_INTEGER_CONSTANT:
      return codegen_load(astNode->integerConstant_intValue);
      break;
    case AST_TERM_VAR:
      return codegen_varTerm(astNode);
      break;
  }
}

static int codegen_expr(const shared_ptr<AstNodeC>& astNode)
{
  int leftReg, rightReg;
  
  if (astNode->expresstion_left)
    leftReg = codegen_expr(astNode->expresstion_left);
  if (astNode->expression_right)
    rightReg = codegen_expr(astNode->expression_right);
  
  if (astNode->type == AST_TERM)
    return codegen_term(astNode);
  
  switch (astNode->expr_infixOpType)
  {
    case AST_INFIX_ADD:
      return codegen_add(leftReg, rightReg);
    case AST_INFIX_SUB:
      return codegen_subtract(leftReg, rightReg);
    case AST_INFIX_MULTIPLY:
      return codegen_multiply(leftReg, rightReg);
    case AST_INFIX_DIVIDE:
      return codegen_divide(leftReg, rightReg);
    case AST_INFIX_EQUALS:
      return codegen_compare(leftReg, rightReg, "sete");
    case AST_INFIX_NOT_EQUALS:
      return codegen_compare(leftReg, rightReg, "setne");
    case AST_INFIX_LESS_THAN:
      return codegen_compare(leftReg, rightReg, "setlt");
    case AST_INFIX_GREATER_THAN:
      return codegen_compare(leftReg, rightReg, "setgt");
    case AST_INFIX_LESS_THAN_EQUALS:
      return codegen_compare(leftReg, rightReg, "setle");
    case AST_INFIX_GREATER_THAN_EQUALS:
      return codegen_compare(leftReg, rightReg, "setge");
  }  
}

static void codegen_statement(const shared_ptr<AstNodeC> astNode)
{

  if (astNode->statement_statementType == AST_STATEMENT_PRINT)
  {
    const uint32_t targetReg = codegen_expr(astNode->statement_statementChild);
    codegen_printRegister(targetReg);
    registers.freeRegister(targetReg);
  }
  if (astNode->statement_statementType == AST_STATEMENT_DECLARATION)
  {
    EMIT2(".comm", astNode->varTerm_globalVariable_variableName + "," +
                                              to_string(astNode->varTerm_globalVariable_variableSize) + "," +
                                              "8");
  }
  if (astNode->statement_statementType == AST_STATEMENT_ASSIGNMENT)
  {
    const int exprReg = codegen_expr(astNode->statement_statementChild);
    EMIT3("movq", "%" + registers.getRegisterName(exprReg),
          astNode->varTerm_globalVariable_variableName + "(" + "%rip" + ")");
    registers.freeRegister(exprReg);
  }
  
  
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