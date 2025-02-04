#include "Lexer.h"

// 引入LLVM支持库
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

// 定义命令行选项，用于接收输入表达式
static llvm::cl::opt<std::string> input(llvm::cl::Positional,
                                        llvm::cl::desc("<input expression>"), // 描述信息
                                        llvm::cl::init(""));                  // 默认值为空字符串

int main(int argc, const char **argv) {
    // 初始化LLVM系统，处理如信号和参数解析等任务
    llvm::InitLLVM llvmInitializer(argc, argv);

    // 解析命令行参数，第二个参数是程序参数，第三个参数是描述信息
    llvm::cl::ParseCommandLineOptions(argc, argv, "simplelexer - a simple lexical analyzer\n");

    // 输出接收到的输入表达式
    llvm::outs() << "Input: \"" << input << "\"\n";

    // 使用输入的表达式初始化词法分析器对象
    Lexer lex(input);

    Token token; // 创建Token对象用于存储当前获取到的token
    // 获取下一个token，并在不是文件结束标记（EOI）的情况下进入循环
    lex.GetNext(token);
    while (token.GetType() != Token::kEOI) {
        // 输出当前token的类型和文本内容
        llvm::outs() << "token type: " << token.GetType() << ", token text: \"" << token.GetText() << "\"\n";

        // 继续获取下一个token，直到遇到EOI
        lex.GetNext(token);
    }

    return 0; // 正常退出
}