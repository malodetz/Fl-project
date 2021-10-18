/*
    A definition of AST node is here

    We use exceptions to detect any kind of error (such as Type Mismatch)
*/
#pragma once

#include "decl.hpp"

#include <llvm/IR/Value.h>

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <optional>
#include <cassert>
#include <unordered_map>
#include <stack>

namespace AST
{
    struct Statement;
    std::stack<Statement *> &StackOfStatements();

    std::stack<std::size_t> &CodeBlockStart();

    namespace details
    {
        DataType GetType(int val);

        DataType GetType(bool val);

        DataType GetType(std::string val);

        bool HasType(const Expression &e, DataType type);

        bool SameType(const Expression &lhs, const Expression &rhs);
        std::string ShowType(DataType type);
    }

    // None must be unreachable
    enum class DataType
    {
        None,
        String,
        Int,
        Bool
    };

    enum class UnaryOpType
    {
        Minus,
        Neg
    };

    enum class BinaryOpType
    {
        Pow,
        Mult,
        Div,
        Sum,
        Sub,
        Leq,
        Les,
        Geq,
        Gre,
        Eq,
        Neq,
        And,
        Or
    };

    struct Node
    {
        virtual ~Node();
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) 
        {
            return nullptr;
        }
    };

    struct Expression : Node
    {
        DataType type;
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override { std::cout << "Expression base class\n"; return nullptr; }
    };

    struct Statement : Node
    {
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override { std::cout << "Statement base class\n"; return nullptr; }
    };

    struct CodeBlock : Node
    {
        StatementList statements;
        CodeBlock(StatementList statements_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct ConstantInt : Expression
    {
        int val;
        ConstantInt(int val_) : val(val_)
        {
            type = DataType::Int;
            std::cerr << "Constructed Constant: " << val_ << std::endl;
        }
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct ConstantString : Expression 
    {
        std::string val;
        ConstantString(std::string val_) : val(val_)
        {
            type = DataType::String;
            std::cerr << "Constructed Constant: " << val_ << std::endl;
        }
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct ConstantBool : Expression
    {
        bool val;
        ConstantBool(bool val_) : val(val_)
        {
            type = DataType::Bool;
            std::cerr << "Constructed Constant: " << val_ << std::endl;
        }
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    // Gonna find value in context
    struct Identifier : public Expression
    {
        std::string name;
        Identifier(DataType type, std::string name_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct UnaryOp : public Expression
    {
        UnaryOpType op;
        Expression &expr;
        UnaryOp(UnaryOpType op_, Expression &expr_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct BinaryOp : public Expression
    {
        BinaryOpType op;
        Expression &lhs;
        Expression &rhs;
        BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    // Kinds of statement

    struct Skip : Statement
    {
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct VarDecl : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarDecl(Identifier *ident_, Expression &expr_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct VarAssign : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarAssign(Identifier *ident_, Expression &expr_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct WhileLoop : Statement
    {
        Expression &expr;
        CodeBlock code_block;
        WhileLoop(Expression &expr_, CodeBlock code_block_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

    struct IfStatement : Statement
    {
        Expression &expr;
        CodeBlock on_if;
        std::optional<CodeBlock> on_else;
        IfStatement(Expression &expr_, CodeBlock on_if_, std::optional<CodeBlock> on_else_);
        virtual llvm::Value *CodeGen(codegen::CodeGenContext &context) override;
    };

}
