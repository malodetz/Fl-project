#pragma once
#include "decl.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

namespace parsingcontext {
    struct ParsingContext {
        std::unordered_map<std::string, AST::Identifier *> variables;
        AST::StatementList curCodeBlock;
    
        AST::Identifier *loadIdent(const std::string &name)
        {
            auto resp = variables.find(name);
            if (resp == variables.end())
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

            if (auto iter = variables.find(name); iter != variables.end())
            {
                throw std::runtime_error("[Internal error] Trying to store an already storead value!");
            }
            variables[std::move(name)] = ident;
        }

        AST::StatementList GetBlockAndClear() {
            return std::exchange(curCodeBlock, {});
        }

    };
}