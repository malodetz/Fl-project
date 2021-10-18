#pragma once
#include "decl.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <stack>

namespace parsingcontext {
    struct ParsingContext {
        std::unordered_map<std::string, AST::Identifier *> variables;
        std::stack<AST::StatementList> StackOfCodeBlocks;
    

        ParsingContext() : StackOfCodeBlocks({AST::StatementList{}}) {
            assert(StackOfCodeBlocks.size() == 1);
        }
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
            std::cout << "Before access" << std::endl;
            variables[name] = ident;
        }

        AST::StatementList GetBlockAndClear() {
            auto resp = std::move(StackOfCodeBlocks.top());
            StackOfCodeBlocks.pop();
            return resp;;
        }

        void AddStatement(AST::Statement * stmt) {
            StackOfCodeBlocks.top().push_back(stmt);
        }

    };
}