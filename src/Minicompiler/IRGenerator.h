#pragma once

#include "AST.h" // 包含抽象语法树（AST）相关的头文件

/**
 * @class IRGenerator
 * @brief 用于将抽象语法树转换为中间表示（Intermediate Representation, IR）的类。
 *
 * 此类提供了一个 Generate 方法，该方法接收一个指向 AST 根节点的指针，
 * 并根据 AST 的结构生成相应的中间代码表示。这种中间表示可以进一步用于优化或生成目标代码。
 */
class IRGenerator {
public:
    /**
     * @brief 根据给定的 AST 生成中间代码表示。
     *
     * 该方法遍历整个 AST，根据节点类型生成相应的 IR。这通常涉及到递归地处理子节点，
     * 并为每种类型的表达式或语句生成对应的 IR 指令。
     *
     * @param Tree 指向 AST 根节点的指针。
     */
    void Generate(AST *Tree);
};