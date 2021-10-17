#pragma once
#include <unordered_map> 
#include "decl.hpp"
#include <llvm/IR/Value.h>

namespace codegen
{
    struct CodeGenContext
    {
        std::unordered_map<std::string, llvm::Value *> variables;
        llvm::Function * mainFunction;
        AST::CodeBlock * block;
    };
    
} // namespace codegen
