#include <iostream>

// 使用 extern "C" 声明一个外部块，使得其中的函数可以按照 C 语言的命名规则进行链接。
extern "C"
{
    // 定义 CalculatorWrite 函数，接收一个整型参数，并输出该结果。
    void calc_write(int result)
    {
        std::cout << "The result is: " << result << std::endl;
    }

    // 定义 CalculatorRead 函数，接收一个指向字符数组的指针（即变量名），并返回用户输入的整数值。
    int calc_read(const char* variableName)
    {
        std::cout << "Enter a value for " << variableName << ": ";

        int value;
        std::cin >> value;

        return value;
    }
}