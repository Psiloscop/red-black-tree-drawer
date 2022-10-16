#pragma once

enum RBColor
{
    BLACK = false,
    RED = true
};

class RBNode
{
public:
    int value;

    RBColor color;

    RBNode* parent;
    RBNode* left;
    RBNode* right;

    RBNode(RBNode* parent, RBNode* left, RBNode* right, int value, RBColor color)
        : parent{parent}, left{left}, right{right}, value{value}, color{color}
    {}
};

#define NIL &sentinel
RBNode sentinel = { NIL, NIL, NIL, 0, RBColor::BLACK };