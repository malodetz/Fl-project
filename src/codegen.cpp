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

        // astBlock->CodeGen(*this);

        llvm::Value * x = llvm::ConstantInt::get(llvmCtx, llvm::APInt(42, 16));
        llvm::Value * y = llvm::ConstantInt::get(llvmCtx, llvm::APInt(42, 16));
        builder->CreateMul(x, y);

        // llvm::ReturnInst::Create(llvmCtx, basicBlock);

        builder->CreateAdd(x, y);

        builder->CreateRetVoid();


        llvm::legacy::PassManager pm;
        pm.add(llvm::createPrintModulePass(llvm::outs()));
        pm.run(*module);



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

namespace AST
{
}
