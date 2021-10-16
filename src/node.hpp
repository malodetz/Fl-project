/*
    A definition of AST node is here

    We use exceptions to detect any kind of error (such as Type Mismatch)
*/
#pragma once

// #include <llvm/IR/Value.h>

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <optional>
#include <cassert>
#include <unordered_map>

namespace AST
{
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

    struct CodeGenContext;
    struct Node;
    struct Expression;
    struct Statement;
    struct Identifier;

    using StatementList = std::vector<Statement *>;

    static StatementList CurCodeBlock;
    // TODO make .hpp file with ^^^^^^^^^^

    std::string ShowType(DataType type);

    std::unordered_map<std::string, Identifier *> &Variables();

    void PrintVarDict();

    struct Node
    {
        virtual ~Node();
        // virtual llvm::Value *CodeGen(CodeGenContext &context)
        // {
        //     return nullptr;
        // }
    };

    struct Expression : Node
    {
        DataType type;
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    bool HasType(const Expression &e, DataType type);

    bool SameType(const Expression &lhs, const Expression &rhs);

    struct Statement : Node
    {
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct CodeBlock : Node
    {
        StatementList statements;
        CodeBlock(StatementList statements_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    // Kinds of expression


    DataType GetType(int val);
    DataType GetType(bool val);
    DataType GetType(std::string val);

    template <class LiteralType>
    struct Constant : Expression
    {
        LiteralType val;
        Constant(LiteralType val_) : val(val_)
        {
            type = GetType(val_);
            std::cerr << "Constructed Constant: " << val_ << std::endl;
        }
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };


    // Gonna find value in context
    struct Identifier : public Expression
    {
        std::string name;
        Identifier(DataType type, std::string name_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct UnaryOp : public Expression
    {
        UnaryOpType op;
        Expression &expr;
        UnaryOp(UnaryOpType op_, Expression &expr_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct BinaryOp : public Expression
    {
        BinaryOpType op;
        Expression &lhs;
        Expression &rhs;
        BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    // Kinds of statement

    struct Skip : Statement
    {
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct VarDecl : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarDecl(Identifier *ident_, Expression &expr_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct VarAssign : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarAssign(std::string &ident_name, Expression &expr_);
        // virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct WhileLoop : Statement
    {
        Expression &expr;
        CodeBlock code_block;
        WhileLoop(Expression &expr_, CodeBlock code_block_);
    };

    struct IfStatement : Statement
    {
        Expression &expr;
        CodeBlock on_if;
        std::optional<CodeBlock> on_else;
        IfStatement(Expression &expr_, CodeBlock on_if_, std::optional<CodeBlock> on_else_);
    };

}
