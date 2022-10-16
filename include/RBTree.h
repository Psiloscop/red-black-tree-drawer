#pragma once

#include "RBNode.h"

class RBTree
{
protected:

    void rotateLeft(RBNode* node)
    {
        RBNode* pivot = node->right;

        if (node->parent != NIL)
        {
            node->parent->left == node
                ? node->parent->left = pivot
                : node->parent->right = pivot;
        }

        pivot->parent = node->parent;
        node->parent = pivot;
        node->right = pivot->left;

        if (pivot->left != NIL)
        {
            pivot->left->parent = node;
        }

        pivot->left = node;

        if (node == this->root)
        {
            this->root = pivot;
        }
    }

    void rotateRight(RBNode* node)
    {
        RBNode* pivot = node->left;

        if (node->parent != NIL)
        {
            node->parent->left == node
                ? node->parent->left = pivot
                : node->parent->right = pivot;
        }

        pivot->parent = node->parent;
        node->parent = pivot;
        node->left = pivot->right;

        if (pivot->right != NIL)
        {
            pivot->right->parent = node;
        }

        pivot->right = node;

        if (node == this->root)
        {
            this->root = pivot;
        }
    }

    void fixInsertion(RBNode* node)
    {
        while (node != this->root && node->parent->color == RBColor::RED)
        {
            if (node->parent == node->parent->parent->left)
            {
                RBNode *uncle = node->parent->parent->right;

                if (uncle != NIL && uncle->color == RBColor::RED)
                {
                    /* uncle exists and uncle is RED */
                    uncle->color = RBColor::BLACK;
                    node->parent->color = RBColor::BLACK;
                    node->parent->parent->color = RBColor::RED;
                    node = node->parent->parent;
                }
                else
                {
                    /* uncle doesn't exist or uncle is BLACK */
                    if (node == node->parent->right)
                    {
                        /* make node a left child */
                        node = node->parent;

                        this->rotateLeft(node);
                    }

                    /* recolor and rotate */
                    node->parent->color = RBColor::BLACK;
                    node->parent->parent->color = RBColor::RED;

                    this->rotateRight(node->parent->parent);
                }
            }
            else
            {
                RBNode *uncle = node->parent->parent->left;

                if (uncle != NIL && uncle->color == RED)
                {
                    /* uncle is RED */
                    uncle->color = RBColor::BLACK;
                    node->parent->color = RBColor::BLACK;
                    node->parent->parent->color = RBColor::RED;

                    node = node->parent->parent;
                }
                else
                {
                    /* uncle is BLACK */
                    if (node == node->parent->left)
                    {
                        node = node->parent;

                        this->rotateRight(node);
                    }

                    node->parent->color = RBColor::BLACK;
                    node->parent->parent->color = RBColor::RED;

                    this->rotateLeft(node->parent->parent);
                }
            }
        }

        this->root->color = RBColor::BLACK;
    }

    void fixRemove(RBNode* node)
    {
        RBNode *parent, *brother;

        while (node != this->root && node->color == RBColor::BLACK)
        {
            parent = node->parent;

            if (node == parent->left)
            {
                brother = node->parent->right;

                if (brother->color == RBColor::RED)
                {
                    brother->color = RBColor::BLACK;
                    parent->color = RBColor::RED;

                    this->rotateLeft(parent);

                    brother = parent->right;

                }

                if (brother->left->color == RBColor::BLACK && brother->right->color == RBColor::BLACK)
                {
                    brother->color = RBColor::RED;

                    node = parent;
                }
                else
                {
                    if (brother->right->color == RBColor::BLACK)
                    {
                        brother->left->color = RBColor::BLACK;
                        brother->color = RBColor::RED;

                        this->rotateRight(brother);

                        brother = node->parent->right;
                    }

                    brother->color = parent->color;
                    brother->right->color = RBColor::BLACK;
                    parent->color = RBColor::BLACK;

                    this->rotateLeft(parent);

                    node = this->root;
                }
            }
            else
            {
                brother = node->parent->left;

                if (brother->color == RBColor::RED)
                {
                    brother->color = RBColor::BLACK;
                    parent->color = RBColor::RED;

                    this->rotateRight(parent);

                    brother = parent->left;

                }

                if (brother->left->color == RBColor::BLACK && brother->right->color == RBColor::BLACK)
                {
                    brother->color = RBColor::RED;

                    node = parent;
                }
                else
                {
                    if (brother->left->color == RBColor::BLACK)
                    {
                        brother->right->color = RBColor::BLACK;
                        brother->color = RBColor::RED;

                        this->rotateLeft(brother);

                        brother = node->parent->left;
                    }

                    brother->color = parent->color;
                    brother->left->color = RBColor::BLACK;
                    parent->color = RBColor::BLACK;

                    this->rotateRight(parent);

                    node = this->root;
                }
            }
        }
    }

public:

    RBNode* root = NIL;

    RBTree() = default;

    RBNode* findNode(int searchValue)
    {
        RBNode* cursor = this->root;

        while (cursor != NIL && cursor->value != searchValue)
        {
            cursor = searchValue < cursor->value ? cursor->left : cursor->right;
        }

        return cursor;
    }

    void insert(int value)
    {
        auto* node = new RBNode(NIL, NIL, NIL, value, RBColor::RED);

        if (this->root == NIL)
        {
            this->root = node;
        }
        else
        {
            RBNode *parent, *child = this->root;

            do
            {
                parent = child;
                child = value < parent->value ? parent->left : parent->right;
            }
            while (child != NIL);

            node->parent = parent;

            if (value < parent->value)
            {
                parent->left = node;
            }
            else
            {
                parent->right = node;
            }
        }

        this->fixInsertion(node);
    }

    void remove(int value)
    {
        RBNode *node = this->findNode(value), *cursor, *cursorChild;

        if (node == NIL)
        {
            return;
        }

        if (node->left != NIL && node->right != NIL)
        {
            cursor = node->right;

            while (cursor->left != NIL)
            {
                cursor = cursor->left;
            }
        }
        else
        {
            cursor = node;
        }

        cursorChild = cursor->left != NIL ? cursor->left : cursor->right;
        cursorChild->parent = cursor->parent;

        if (cursor->parent != NIL)
        {
            if (cursor == cursor->parent->left)
            {
                cursor->parent->left = cursorChild;
            }
            else
            {
                cursor->parent->right = cursorChild;
            }
        }
        else
        {
            this->root = cursorChild;
        }

        if (node->value != cursor->value)
        {
            node->value = cursor->value;
        }

        if (cursor->color == RBColor::BLACK)
        {
            this->fixRemove(cursor);
        }

        delete cursor;
    }

    void clear(RBNode* node = NIL)
    {
        if (node == NIL)
        {
            node = this->root;
        }

        if (node->left != NIL)
        {
            this->clear(node->left);
        }

        if (node->right != NIL)
        {
            this->clear(node->right);
        }

        if (this->root == node)
        {
            this->root = NIL;
        }

        delete node;
    }
};