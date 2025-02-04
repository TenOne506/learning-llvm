#include "Parser.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

// 匿名命名空间，用于定义本地使用的类或函数
namespace
{
    // ASTPrinter 类继承自 ASTVisitor，用于打印 AST 的结构
    class ASTPrinter : public ASTVisitor
    {
    public:
        // 实现 Visit 方法来处理 Factor 节点
        virtual void Visit(Factor& node) override
        {
            llvm::outs() << "Factor: " << node.GetVal() << "\n"; // 打印因子值
        };

        // 实现 Visit 方法来处理 BinaryOp 节点
        virtual void Visit(BinaryOp& Node) override
        {
            // 这里有重复调用 GetDisplayText()，可能是误写
            llvm::outs() << "\n"
                         << Node.GetDisplayText() << Node.GetDisplayText() << Node.GetDisplayText()
                         << Node.GetDisplayText() << "\n";
            llvm::outs() << "BinaryOp: " << Node.GetDisplayText() << "\n";

            // 递归地访问左侧子节点
            if (Node.GetLeft())
            {
                Node.GetLeft()->Accept(*this);
            }

            // 递归地访问右侧子节点
            if (Node.GetRight())
            {
                Node.GetRight()->Accept(*this);
            }

            llvm::outs() << Node.GetDisplayText() << Node.GetDisplayText() << Node.GetDisplayText()
                         << Node.GetDisplayText() << "\n\n";
        };

        // 实现 Visit 方法来处理 WithDeclaration 节点
        virtual void Visit(WithDeclaration& Node) override
        {
            // 打印 'with' 声明中的变量列表
            llvm::outs() << "with variables: ";
            for (auto var : Node)
            {
                llvm::outs() << var << " ";
            }

            llvm::outs() << "\n";

            // 递归地访问与 'with' 关联的表达式
            if (Node.getExpr())
            {
                Node.getExpr()->Accept(*this);
            }
        };
    };
} // namespace

// 定义命令行选项，用于接收输入表达式
static llvm::cl::opt<std::string> input(llvm::cl::Positional, 
                                        llvm::cl::desc("<input expression>"), // 描述信息
                                        llvm::cl::init("")); // 默认值为空字符串

int main(int argc, const char** argv)
{
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
    AST* tree = parser.Parse();
    
    // 检查是否有语法错误或者 AST 构建失败
    if (!tree || parser.HasError())
    {
        llvm::errs() << "Syntax errors occurred\n";
        return 1; // 返回非零值表示程序异常结束
    }

    // 创建 ASTPrinter 对象并使用它来打印 AST 结构
    ASTPrinter printer;
    tree->Accept(printer);

    return 0; // 正常退出
}