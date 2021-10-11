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
#include <optional>

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

    using StatementList = std::vector<Statement *>;

    // TODO make .hpp file with ^^^^^^^^^^

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
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
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
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct CodeBlock : Node
    {
        StatementList statements;
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    // Kinds of expression

    template <class LiteralType>
    struct Constant : Expression
    {
        LiteralType val;
        Constant(LiteralType val_) : val(val_)
        {
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    // Gonna find value in context
    struct Identifier : public Expression
    {
        std::string name;
        Identifier(std::string name_) : name(std::move(name_)) {}
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
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
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
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
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    // Kinds of statement

    struct Skip : Statement
    {
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct VarDecl : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarDecl(Identifier *ident_, Expression &expr_) : ident(ident_),
                                                         expr(expr_)
        {
            if (ident == nullptr)
            {
                throw std::runtime_error("Nullptr ident in VarDecl");
            }
            if (!SameType(*ident, expr))
            {
                throw std::runtime_error("Mismatched typed in VarDecl");
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct VarAssign : Statement
    {
        Identifier *ident;
        Expression &expr;
        VarAssign(Identifier *ident_, Expression &expr_) : ident(ident_),
                                                           expr(expr_)
        {
            if (ident == nullptr)
            {
                throw std::runtime_error("Nullptr ident in VarAssign");
            }
            if (!SameType(*ident, expr))
            {
                throw std::runtime_error("Mismatched typed in VarAssign");
            }
        }
        virtual llvm::Value *CodeGen(CodeGenContext &context) { return nullptr; }
    };

    struct WhileLoop : Statement
    {
        Expression &expr;
        CodeBlock code_block;
        WhileLoop(Expression &expr_, CodeBlock code_block_) : expr(expr_),
                                                              code_block(std::move(code_block_))
        {
            if (!HasType(expr, DataType::Bool))
            {
                throw std::runtime_error("Non-bool expr in WhileLoop");
            }
        }
    };

    struct IfStatement : Statement
    {
        Expression &expr;
        CodeBlock on_if;
        std::optional<CodeBlock> on_else;
        IfStatement(Expression &expr_, CodeBlock on_if_, std::optional<CodeBlock> on_else_) : expr(expr_),
                                                                                              on_if(std::move(on_if_)),
                                                                                              on_else(std::move(on_else_))
        {
            if (!HasType(expr, DataType::Bool))
            {
                throw std::runtime_error("Non-bool expr in WhileLoop");
            }
        }
    };

}
