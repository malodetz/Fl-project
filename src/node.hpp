/*
    A definition of AST node is here

    We use exceptions to detect any kind of error (such as Type Mismatch)
*/
#pragma once

#include <llvm/IR/Value.h>

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <memory>

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

    std::string ShowType(DataType type)
    {
        switch (type)
        {
        case DataType::None:
            return "None";
            break;
        case DataType::String:
            return "String";
            break;
        case DataType::Int:
            return "Int";
            break;
        case DataType::Bool:
            return "Bool";
            break;
        default:
            llvm_unreachable("ShowType: unknown DataType!");
            return "Wtf?\n";
            break;
        }
    }

    struct CodeGenContext;

    struct Node
    {
        virtual ~Node() {}
        virtual llvm::Value *CodeGen(CodeGenContext &context)
        {
            return nullptr;
        }
    };

    struct Expression : Node
    {
        DataType type;
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    bool HasType(const Expression &e, DataType type)
    {
        return e.type == type;
    }

    bool SameType(const Expression &lhs, const Expression &rhs)
    {
        return lhs.type == rhs.type;
    }

    struct Statement : Node
    {
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    // Kinds of expression

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
    struct Identifier : public Expression
    {
        std::string name;
        Identifier(std::string name_) : name(std::move(name_)) {}
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct UnaryOp : public Expression
    {
        UnaryOpType op;
        Expression &expr;
        UnaryOp(UnaryOpType op_, Expression &expr_) : op(op_), expr(expr_)
        {
            const char *errorMsg = "UnaryOp with wrong type";
            switch (op)
            {
            case UnaryOpType::Minus:
            {
                if (!HasType(expr, DataType::Int))
                {
                    throw std::runtime_error(errorMsg);
                }
                break;
            }
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct BinaryOp : public Expression
    {
        BinaryOpType op;
        Expression &lhs;
        Expression &rhs;
        BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_) : op(op_), lhs(lhs_), rhs(rhs_)
        {
            const char *errorMsg = "BinOp with wrong types";
            switch (op)
            {
            case BinaryOpType::Pow:
            case BinaryOpType::Mult:
            case BinaryOpType::Div:
            case BinaryOpType::Sub:
            case BinaryOpType::Leq:
            case BinaryOpType::Les:
            case BinaryOpType::Geq:
            case BinaryOpType::Gre:
            {
                if (!HasType(lhs, DataType::Int) || !HasType(rhs, DataType::Int))
                {
                    throw std::runtime_error(errorMsg);
                }
                break;
            }
            case BinaryOpType::Sum:
            {
                bool bothInt = HasType(lhs, DataType::Int) && HasType(rhs, DataType::Int);
                bool bothStr = HasType(lhs, DataType::Int) && HasType(rhs, DataType::Int);
                if (!bothInt && !bothStr)
                {
                    throw std::runtime_error(errorMsg);
                }
                break;
            }
            case BinaryOpType::Eq:
            case BinaryOpType::Neq:
            {
                if (!SameType(lhs, rhs))
                {
                    throw std::runtime_error(errorMsg);
                }
                break;
            }
            case BinaryOpType::And:
            case BinaryOpType::Or:
            {
                if (!HasType(lhs, DataType::Bool) || !HasType(lhs, DataType::Bool))
                {
                    throw std::runtime_error(errorMsg);
                }
            }
            default:
                std::cerr << "Unknown BinOp\n";
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    // Kinds of statement

    struct Skip : Statement
    {
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct VarDecl : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarDecl(Identifier * ident_, Expression &expr_) :
        ident(ident_),
        expr(expr_)
        {
            if (ident == nullptr) {
                throw std::runtime_error("Nullptr ident in VarDecl");
            }
            if (!SameType(*ident, expr)) {
                throw std::runtime_error("Mismatched typed in VarDecl");
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    struct VarAssign : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarAssign(Identifier * ident_, Expression &expr_) :
        ident(ident_),
        expr(expr_)
        {
            if (ident == nullptr) {
                throw std::runtime_error("Nullptr ident in VarAssign");
            }
            if (!SameType(*ident, expr)) {
                throw std::runtime_error("Mismatched typed in VarAssign");
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

}
