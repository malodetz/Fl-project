#include <exception>
#include <string>
#include <unordered_map>
#include "node.hpp"
#include <llvm/IR/Value.h>

namespace AST
{
    namespace details
    {

        DataType GetType(int val)
        {
            return DataType::Int;
        }

        DataType GetType(bool val)
        {
            return DataType::Bool;
        }

        DataType GetType(std::string val)
        {
            return DataType::String;
        }

        bool HasType(const Expression &e, DataType type)
        {
            return e.type == type;
        }

        bool SameType(const Expression &lhs, const Expression &rhs)
        {
            return lhs.type == rhs.type;
        }
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
                break;
            }
        }
    }

    Node::~Node() {}


    Identifier::Identifier(DataType type_, std::string name_) : name(std::move(name_))
    {
        type = std::move(type_);
        std::cerr << "Identifier " << name << " created" << '\n';

        // Variables[name] = this;
        std::cerr << "[Variables] <--- " << name << std::endl;
    }

    CodeBlock::CodeBlock(StatementList statements_) : statements(std::move(statements_))
    {
        std::cerr << "_____CodeBlock created_____" << std::endl;
    }

    UnaryOp::UnaryOp(UnaryOpType op_, Expression &expr_) : op(op_), expr(expr_)
    {
        const char *errorMsg = "UnaryOp with wrong type";
        switch (op)
        {
        case UnaryOpType::Minus:
        {
            type = DataType::Int;
            if (!details::HasType(expr, DataType::Int))
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        case UnaryOpType::Neg:
        {
            type = DataType::Bool;
            if (!details::HasType(expr, DataType::Bool))
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        }
    }

    BinaryOp::BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_) : op(op_), lhs(lhs_), rhs(rhs_)
    {
        const char *errorMsg = "BinOp with wrong types";

        // type checking
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
            if (!details::HasType(lhs, DataType::Int) || !details::HasType(rhs, DataType::Int))
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        case BinaryOpType::Sum:
        {
            bool bothInt = details::HasType(lhs, DataType::Int) && details::HasType(rhs, DataType::Int);
            bool bothStr = details::HasType(lhs, DataType::String) && details::HasType(rhs, DataType::String);
            if (!bothInt && !bothStr)
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        case BinaryOpType::Eq:
        case BinaryOpType::Neq:
        {
            if (!details::SameType(lhs, rhs))
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        case BinaryOpType::And:
        case BinaryOpType::Or:
        {
            if (!details::HasType(lhs, DataType::Bool) || !details::HasType(lhs, DataType::Bool))
            {
                throw std::runtime_error(errorMsg);
            }
            break;
        }
        default:
            std::cerr << "Unknown BinOp\n";
        }

        // calculating result type
        switch (op)
        {
        case BinaryOpType::Pow:
        case BinaryOpType::Mult:
        case BinaryOpType::Div:
        case BinaryOpType::Sub:
        case BinaryOpType::Sum:
        {
            type = lhs.type;
            assert(lhs.type == rhs.type);
            break;
        }
        case BinaryOpType::Leq:
        case BinaryOpType::Les:
        case BinaryOpType::Geq:
        case BinaryOpType::Gre:
        case BinaryOpType::Eq:
        case BinaryOpType::Neq:
        case BinaryOpType::And:
        case BinaryOpType::Or:
        {
            type = DataType::Bool;
            break;
        }
        }
    }

    VarDecl::VarDecl(Identifier *ident_, Expression &expr_) : ident(ident_),
                                                              expr(expr_)
    {
        if (ident == nullptr)
        {
            throw std::runtime_error("Nullptr ident in VarDecl");
        }
        if (!details::SameType(*ident, expr))
        {
            throw std::runtime_error("Mismatched typed in VarDecl");
        }
    }


    VarAssign::VarAssign(Identifier *ident_, Expression &expr_) : expr(expr_),
                                                                  ident(ident_)
    {
        if (ident == nullptr)
        {
            throw std::runtime_error("Nullptr ident in VarAssign");
        }
        if (!details::SameType(*ident, expr))
        {
            throw std::runtime_error("Mismatched typed in VarAssign");
        }
        std::cerr << ident->name << " assigned" << std::endl;
    }

    WhileLoop::WhileLoop(Expression &expr_, CodeBlock code_block_) : expr(expr_),
                                                                     code_block(std::move(code_block_))
    {
        if (!details::HasType(expr, DataType::Bool))
        {
            throw std::runtime_error("Non-bool expr in WhileLoop");
        }
        std::cerr << "While cycle created" << std::endl;
    }

    IfStatement::IfStatement(Expression &expr_, CodeBlock on_if_, std::optional<CodeBlock> on_else_) : expr(expr_),
                                                                                                       on_if(std::move(on_if_)),
                                                                                                       on_else(std::move(on_else_))
    {
        if (!details::HasType(expr, DataType::Bool))
        {
            throw std::runtime_error("Non-bool expr in WhileLoop");
        }
        std::cerr << "If statement created" << std::endl;
    }
}