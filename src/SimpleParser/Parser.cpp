#include "Parser.h"

AST *Parser::Parse() {
    // 调用 ParseCalc 方法来解析整个计算表达式，并返回生成的 AST 树
    AST *tree = ParseCalc();
    // IsNextTokenOfType(Token::kEOI); // 这行被注释掉了，可能是用于调试或验证是否到达输入结尾

    return tree; // 返回最终构建的 AST 树
}

AST *Parser::ParseCalc() {
    Expr *expr;
    llvm::SmallVector<llvm::StringRef, 8> variables; // 存储 'with' 声明中的变量名列表

    // 如果当前 token 是 'with' 关键字
    if (token.Is(Token::kKeywordWith)) {
        Advance(); // 向前推进到下一个 token
        // 检查下一个 token 是否为标识符，如果不是则跳转至错误处理标签
        if (IsNextTokenOfType(Token::kIdent)) {
            goto errorTag;
        }

        variables.push_back(token.GetText()); // 添加标识符到变量列表
        Advance();                            // 推进到下一个 token
        // 处理逗号分隔的多个标识符
        while (token.Is(Token::kComma)) {
            Advance();
            if (IsNextTokenOfType(Token::kIdent)) {
                goto errorTag;
            }

            variables.push_back(token.GetText());
            Advance();
        }

        // 消耗冒号，如果失败则跳转至错误处理标签
        if (Consume(Token::kColon)) {
            goto errorTag;
        }
    }

    // 解析表达式
    expr = ParseExpr();
    // 检查是否意外到达文件结束标记（EOI）
    if (IsNextTokenOfType(Token::kEOI)) {
        goto errorTag;
    }

    // 如果没有 'with' 变量，则直接返回表达式；否则，返回一个 WithDeclaration 对象
    if (variables.empty()) {
        return expr;
    } else {
        return new WithDeclaration(variables, expr);
    }

errorTag:
    // 错误处理：继续读取直到到达 EOI 或遇到可接受的符号
    while (token.GetType() != Token::kEOI) {
        Advance();
    }

    return nullptr; // 返回空指针表示解析失败
}

// 解析表达式，包括加法和减法运算
Expr *Parser::ParseExpr() {
    Expr *left = ParseTerm(); // 解析项
    // 处理连续的加法或减法操作
    while (token.IsOneOf(Token::kPlus, Token::kMinus)) {
        BinaryOp::Operator op = token.Is(Token::kPlus) ? BinaryOp::kPlus : BinaryOp::kMinus;

        Advance();                            // 推进到下一个 token
        Expr *right = ParseTerm();            // 解析右侧的项
        left = new BinaryOp(op, left, right); // 创建一个新的二元操作节点
    }

    return left; // 返回表达式树
}

// 解析项，主要处理乘法和除法运算
Expr *Parser::ParseTerm() {
    Expr *left = ParseFactor(); // 解析因子
    // 处理连续的乘法或除法操作
    while (token.IsOneOf(Token::kStar, Token::kSlash)) {
        BinaryOp::Operator op = token.Is(Token::kStar) ? BinaryOp::kMultiple : BinaryOp::kDivide;

        Advance();                            // 推进到下一个 token
        Expr *right = ParseFactor();          // 解析右侧的因子
        left = new BinaryOp(op, left, right); // 创建一个新的二元操作节点
    }

    return left; // 返回项树
}

// 解析因子，如数字、变量或括号内的表达式
Expr *Parser::ParseFactor() {
    Expr *expr = nullptr;
    switch (token.GetType()) {
    case Token::kNumber:
        expr = new Factor(Factor::kNumber, token.GetText()); // 创建数值因子
        Advance();
        break;

    case Token::kIdent:
        expr = new Factor(Factor::kIdent, token.GetText()); // 创建标识符因子
        Advance();
        break;

    case Token::kLeftParen:
        Advance();
        expr = ParseExpr();               // 解析括号内的表达式
        if (!Consume(Token::kRightParen)) // 确保匹配右括号
        {
            break;
        }

    default:
        if (!expr) {
            AddError(); // 如果无法识别 token 类型，报告错误
        }

        // 错误恢复：跳过不期望的 token 直到找到合法的符号
        while (
            !token.IsOneOf(Token::kRightParen, Token::kStar, Token::kPlus, Token::kMinus, Token::kSlash, Token::kEOI)) {
            Advance();
        }

        break;
    }

    return expr; // 返回因子
}