#pragma once
#include "decl.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include <unordered_map>
#include <unordered_set>

namespace codegen
{
    struct CodeGenContext
    {
        
        std::unordered_map<std::string, llvm::Value *> variables;
        llvm::Function *mainFunction;
        AST::CodeBlock *block;

        

        // void generateCode();
        // llvm::GenericValue runCode();
    };

} // namespace codegen
