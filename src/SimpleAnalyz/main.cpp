#include "Parser.h"
#include "SemanticAnalyzer.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

// 定义命令行选项，用于接收输入表达式
static llvm::cl::opt<std::string> input(llvm::cl::Positional,
                                        llvm::cl::desc("<input expression>"), // 描述信息
                                        llvm::cl::init(""));                  // 默认值为空字符串

int main(int argc, const char **argv) {
    // 初始化 LLVM 系统，处理如信号和参数解析等任务
    llvm::InitLLVM llvmInitializer(argc, argv);

    // 解析命令行参数，第二个参数是程序参数，第三个参数是描述信息
    llvm::cl::ParseCommandLineOptions(argc, argv, "SimpleParser - a simple code parser\n");

    // 输出接收到的输入表达式
    llvm::outs() << "Input: \"" << input << "\"\n";

    // 使用输入的表达式初始化词法分析器对象
    Lexer lexer(input);
    // 使用词法分析器初始化解析器对象
    Parser parser(lexer);
    // 解析输入，生成 AST 树
    AST *tree = parser.Parse();

    // 检查是否有语法错误或者 AST 构建失败
    if (!tree || parser.HasError()) {
        llvm::errs() << "Syntax errors occurred\n";
        return 1; // 返回非零值表示程序异常结束
    }

    // 创建 SemanticAnalyzer 对象并使用它进行语义分析
    SemanticAnalyzer semanticAnalyzer;
    // 如果语义分析发现任何错误，则输出错误信息并返回非零值
    if (semanticAnalyzer.Analysis(tree)) {
        llvm::errs() << "Semantic errors occurred\n";
        return 1;
    }

    // 如果没有语义错误，则输出成功信息
    llvm::outs() << "Semantic check passed\n\n";
    return 0; // 正常退出
}