// #include "decl.hpp"
#include "codegen.hpp"

#include "node.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Operator.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>

#include <iostream>
#include <vector>
#include <cassert>

namespace codegen
{
    CodeGenContext::CodeGenContext() : module(new llvm::Module("main", llvmCtx))
    {
    }

    void CodeGenContext::generateCode()
    {
        std::cout << "Start generating code...\n";
        std::vector<llvm::Type *> argTypes;

        builder = new llvm::IRBuilder<>(llvmCtx);

        llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(llvmCtx), llvm::makeArrayRef(argTypes), false);
        mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::InternalLinkage, "main", module);
        basicBlock = llvm::BasicBlock::Create(llvmCtx, "entry", mainFunction);
        builder->SetInsertPoint(basicBlock);

        assert(astBlock);


        std::cout << "AST block size = " << astBlock->statements.size() << '\n';

        astBlock->CodeGen(*this);

        builder->CreateRetVoid();

        llvm::raw_os_ostream out(std::cout);
        module->print(out, nullptr);

        std::cout << "Code generated..\n";
    }

    llvm::GenericValue CodeGenContext::runCode()
    {
        std::cout << "Running code\n";
        llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
        ee->finalizeObject();
        std::cout << "After finalize.." << std::endl;
        std::vector<llvm::GenericValue> noargs;
        llvm::GenericValue v = ee->runFunction(mainFunction, noargs);
        std::cout << "After run func.." << std::endl;
        std::cout << "Code was run.\n";
        return {};
    }
}

namespace
{
    llvm::Type *getType(AST::Identifier *id, llvm::LLVMContext &ctx)
    {
        if (id->type == AST::DataType::Int)
        {
            return llvm::Type::getInt32Ty(ctx);
        }
        if (id->type == AST::DataType::Bool)
        {
            return llvm::Type::getInt1Ty(ctx);
        }
        if (id->type == AST::DataType::String)
        {
            return llvm::Type::getInt8PtrTy(ctx);
        }
        throw std::runtime_error("[internal error] Unknown data type!");
    }
}

namespace AST
{
    llvm::Value *CodeBlock::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating block...\n";
        llvm::Value * last = nullptr;
        int i = 0;
        for (auto &st : statements)
        {
            std::cout << ++i << " statement:\n";
            last = st->CodeGen(context);
        }
        return last;
    }

    // expressions
    llvm::Value *ConstantInt::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating constant i32...\n";
        return context.builder->getInt32(val);
    }

    llvm::Value *ConstantBool::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating constant i1...\n";
        return context.builder->getInt1(val);
    }

    llvm::Value *Identifier::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating Ident with name \"" << name << "\"...\n";
        llvm::AllocaInst *alloca = context.builder->CreateAlloca(getType(this, context.llvmCtx), nullptr, name.c_str());
        context.variables[name] = alloca;
        return alloca;
    }

    // statements
    llvm::Value *VarDecl::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating declaration for " << ident->name << "...\n";
        ident->CodeGen(context);
        
        VarAssign va(ident, expr);
        return va.CodeGen(context);
    }

    llvm::Value *VarAssign::CodeGen(codegen::CodeGenContext &context)
    {
        std::cout << "Generating assignment for " << ident->name << "...\n";
        auto iter = context.variables.find(ident->name);
        if (iter == context.variables.end())
        {
            throw std::runtime_error("[internal error] Variable is not in scope");
        }
        llvm::Value *var = iter->second;
        llvm::Value *expr_val = expr.CodeGen(context);
        
        return context.builder->CreateStore(expr_val, var, false);
    }
}
