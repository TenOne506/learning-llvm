#pragma once

#include "AST.h" // 包含抽象语法树（AST）相关的头文件

// SemanticAnalyzer 类用于执行语义分析，确保程序在语义上是正确的
class SemanticAnalyzer {
public:
    /**
     * @brief 执行对抽象语法树（AST）的语义分析
     *
     * 该方法接收一个指向 AST 根节点的指针，并对其进行遍历和检查，
     * 以确保程序在语义上没有错误，例如类型检查、变量声明检查等。
     *
     * @param tree 指向 AST 根节点的指针
     * @return bool 如果分析成功返回 true，如果发现语义错误则返回 false
     */
    bool Analysis(AST *tree);
};