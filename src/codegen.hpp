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
        std::unordered_map<std::string, AST::Identifier *> variablesRt;
        std::unordered_map<std::string, llvm::Value *> variablesLlvm;
        llvm::Function *mainFunction;
        AST::CodeBlock *block;

        AST::Identifier *loadIdent(const std::string &name)
        {
            auto resp = variablesRt.find(name);
            if (resp == variablesRt.end())
            {
                return nullptr;
            }
            return resp->second;
        }

        void storeIdent(std::string name, AST::Identifier *ident)
        {
            if (!ident)
            {
                throw;
                std::runtime_error("[Internal error] Trying to store a nullptr-Ident!");
            }

            if (auto iter = variablesRt.find(name); iter != variablesRt.end())
            {
                throw std::runtime_error("[Internal error] Trying to store an already storead value!");
            }
            variablesRt[std::move(name)] = ident;
        }

        // void generateCode();
        // llvm::GenericValue runCode();
    };

} // namespace codegen
