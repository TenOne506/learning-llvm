#include "Lexer.h"
#include <cctype>

// 获取下一个Token
void Lexer::GetNext(Token &token) {
    // 跳过当前指针指向的空白字符
    while (*bufferPtr && std::isspace(*bufferPtr)) {
        ++bufferPtr;
    }

    // 如果到达缓冲区末尾，则设置为EOI（End Of Input）类型
    if (!*bufferPtr) {
        token.SetType(Token::kEOI);
        return;
    }

    // 如果是字母开头
    if (std::isalpha(*bufferPtr)) {
        const char *end = bufferPtr + 1;
        // 找到标识符或关键字的结束位置
        while (std::isalpha(*end)) {
            ++end;
        }

        // 根据内容判断是关键字还是普通标识符
        llvm::StringRef name(bufferPtr, end - bufferPtr);
        Token::TokenType type = (name == "with" ? Token::kKeywordWith : Token::kIdent);
        InitializeToken(token, end, type); // 初始化Token对象
        return;
    }
    // 如果是数字开头
    else if (std::isdigit(*bufferPtr)) {
        const char *end = bufferPtr + 1;
        // 找到数字的结束位置
        while (std::isdigit(*end)) {
            ++end;
        }

        InitializeToken(token, end, Token::kNumber); // 初始化Token对象
        return;
    }
    // 处理单个符号
    else {
        // switch (*bufferPtr) {
        // case '+':
        // case '-':
        // case '*':
        // case '/':
        // case '(':
        // case ')':
        // case ':':
        // case ',':
        //     // 根据不同的符号设置相应的Token类型
        //     InitializeToken(token, bufferPtr + 1, static_cast<Token::TokenType>(Token::kPlus + *bufferPtr - '+'));
        //     break;
        // default:
        //     InitializeToken(token, bufferPtr + 1, Token::kUnknown); // 未知字符处理
        // }
        switch (*bufferPtr) {
        case '+':
            InitializeToken(token, bufferPtr + 1, Token::kPlus);
            break;

        case '-':
            InitializeToken(token, bufferPtr + 1, Token::kMinus);
            break;

        case '*':
            InitializeToken(token, bufferPtr + 1, Token::kStar);
            break;

        case '/':
            InitializeToken(token, bufferPtr + 1, Token::kSlash);
            break;

        case '(':
            InitializeToken(token, bufferPtr + 1, Token::kLeftParen);
            break;

        case ')':
            InitializeToken(token, bufferPtr + 1, Token::kRightParen);
            break;

        case ':':
            InitializeToken(token, bufferPtr + 1, Token::kColon);
            break;

        case ',':
            InitializeToken(token, bufferPtr + 1, Token::kComma);
            break;

        default:
            InitializeToken(token, bufferPtr + 1, Token::kUnknown);
        }

        return;
    }
}

// 初始化Token对象的方法
void Lexer::InitializeToken(Token &token, const char *tokenEnd, Token::TokenType type) {
    // 设置Token的类型
    token.SetType(type);
    // 设置Token对应的文本值
    token.SetText(llvm::StringRef(bufferPtr, tokenEnd - bufferPtr));

    // 更新bufferPtr指向下一个待处理的位置
    bufferPtr = tokenEnd;
}