#include <exception>
#include <string>
#include "node.hpp"

namespace AST {
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
            // llvm_unreachable("ShowType: unknown DataType!");
            return "Wtf?\n";
            break;
        }
    }    

    Node::~Node() {}

    DataType GetType(int val) {
        return DataType::Int;
    }

    DataType GetType(bool val) {
        return DataType::Bool;
    }

    DataType GetType(std::string val) {
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

    Identifier::Identifier(DataType type_, std::string name_) : name(std::move(name_)) {
            type = std::move(type_);
            std::cerr << "Identifier " << name << " created" << '\n';  

            Variables[name] = this;
            std::cerr << "[Variables] <--- " << name << std::endl;
    }

    CodeBlock::CodeBlock(StatementList statements_) : statements(std::move(statements_)) {
        std::cerr << "_____CodeBlock created_____" << std::endl;
    }

    UnaryOp::UnaryOp(UnaryOpType op_, Expression &expr_) : op(op_), expr(expr_)
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

        BinaryOp::BinaryOp(Expression &lhs_, BinaryOpType op_, Expression &rhs_) : op(op_), lhs(lhs_), rhs(rhs_)
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

        VarDecl::VarDecl(Identifier *ident_, Expression &expr_) : ident(ident_),
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

        VarAssign::VarAssign(std::string &ident_name, Expression &expr_) :
                                                           expr(expr_)
        {
            ident = Variables[ident_name];
            if (ident == nullptr)
            {
                throw std::runtime_error("Nullptr ident in VarAssign");
            }
            if (!SameType(*ident, expr))
            {
                throw std::runtime_error("Mismatched typed in VarAssign");
            }
            std::cerr << ident_name << " assigned" << std::endl;
        }
        
        WhileLoop::WhileLoop(Expression &expr_, CodeBlock code_block_) : expr(expr_),
                                                              code_block(std::move(code_block_))
        {
            if (!HasType(expr, DataType::Bool))
            {
                throw std::runtime_error("Non-bool expr in WhileLoop");
            }
        }

        IfStatement::IfStatement(Expression &expr_, CodeBlock on_if_, std::optional<CodeBlock> on_else_) : expr(expr_),
                                                                                              on_if(std::move(on_if_)),
                                                                                              on_else(std::move(on_else_))
        {
            if (!HasType(expr, DataType::Bool))
            {
                throw std::runtime_error("Non-bool expr in WhileLoop");
            }
        }
}