/*
    A definition of AST node is here
*/
#pragma once
#include <vector>
#include <string>
#include <llvm/IR/Value.h>

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

    struct Node
    {
        virtual ~Node() {}
        virtual llvm::Value *CodeGen(CodeGenContext &context)
        {
            return nullptr;
        }
    };

    // TODO: add type checking in all Expressions
    struct Expression : Node
    {
        DataType type;
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct Statement : Node
    {
    };

    template <class LiteralType>
    struct Constant : Expression
    {
        LiteralType val;
        Constant(LiteralType val_) : val(val_)
        {
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    // Gonna find value in context
    struct NIdentifier : public Expression
    {
        std::string name;
        NIdentifier(std::string name_) : name(std::move(name_)) {}
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct UnaryOp : public Expression
    {
        UnaryOpType op;
        Expression &expr;
        UnaryOp(UnaryOpType op_, Expression &expr_) : op(op_), expr(expr_) {}
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct BinaryOp : public Expression
    {
        BinaryOpType op;
        Expression &lhs;
        Expression &rhs;
        BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_) : op(op_), lhs(lhs_), rhs(rhs_) {}
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };
}
