#pragma once

#include <vector>

namespace codegen {
    struct CodeGenContext;
}
namespace AST {
    enum class DataType;
    enum class UnaryOpType;
    enum class BinaryOpType;

    struct Node;
    struct Expression;
    struct Statement;
    struct CodeBlock;

    struct Identifier;
    template<class T>
    struct Constant;
    struct UnaryOp;
    struct BinaryOp;

    struct Skip;
    struct VarDecl;
    struct VarAssign;
    struct WhileLoop;
    struct IfStatement;

    using StatementList = std::vector<Statement *>;
}