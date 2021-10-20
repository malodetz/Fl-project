#pragma once

#include "decl.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/LLVMContext.h>

#include <unordered_map>
#include <unordered_set>

namespace codegen {
    struct CodeGenContext {

        llvm::LLVMContext llvmCtx;
        llvm::Module *module;
        llvm::IRBuilder<> *builder{};
        llvm::BasicBlock *basicBlock{}; // sequence of inst
        std::unordered_map<std::string, llvm::Value *> variables;
        llvm::Function *mainFunction = nullptr;
        AST::CodeBlock *astBlock = nullptr; // ast is here

        llvm::BasicBlock *printBlock = nullptr;

        llvm::Value *fmtInt = nullptr;
        llvm::Value *fmtStr = nullptr;
        llvm::FunctionCallee printfCallee;

        llvm::Function *printfF;

        CodeGenContext();

        void generateCode();

        void saveCode(const std::string &output_fname) const;

        llvm::GenericValue runCode() const;
    };

} // namespace codegen
