#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "RBTree.h"

struct DrawableNode
{
    DrawableNode(RBNode* node, float xPos, float yPos): node{node}, xPos{xPos}, yPos{yPos}
    {}

    RBNode* node;

    float xPos;
    float yPos;

    DrawableNode* parent = nullptr;
    DrawableNode* left = nullptr;
    DrawableNode* right = nullptr;
};

class Drawer
{
protected:

    DrawableNode* root = nullptr;

    std::vector<std::vector<float>*> usedXCoords = {};

    sf::Font font;

    std::vector<sf::CircleShape*> circles = {};
    std::vector<sf::VertexArray*> lines = {};
    std::vector<sf::Text*> values = {};

    const float circleDiameter = 75;
    const float circleRadius = circleDiameter / 2;
    const unsigned int fontSize = static_cast<unsigned int>(circleRadius) / 1.5;

    const float offsetX = circleDiameter - (circleDiameter / 100 * 10);
    const float offsetY = circleDiameter + (circleDiameter / 100 * 40);

    float prepareDrawableNode(RBNode* _rbNode, DrawableNode* drNode = nullptr, int _layer = 0, float _offsetX = 0, float _offsetY = 0)
    {
        DrawableNode *drNodeParent = drNode, *drLeftNodeChild = nullptr, *drRightNodeChild = nullptr;
        float newOffsetX = _offsetX, newOffsetY = _offsetY + this->offsetY;
        float parentOffsetX, leftChildOffsetX, rightChildOffsetX;
        int usedXCoordsSize = static_cast<int>(this->usedXCoords.size()) - 1;

        if (_layer > usedXCoordsSize)
        {
            this->usedXCoords.push_back(new std::vector<float>(0));
        }

        if (!this->usedXCoords[_layer]->empty() && this->usedXCoords[_layer]->back() + this->offsetX >= newOffsetX)
        {
            newOffsetX = this->usedXCoords[_layer]->back();
            newOffsetX += this->offsetX * 2;
        }

        leftChildOffsetX = newOffsetX - this->offsetX;
        rightChildOffsetX = newOffsetX + this->offsetX;

        if (_rbNode->left != NIL)
        {
            drLeftNodeChild = new DrawableNode(_rbNode->left, leftChildOffsetX, newOffsetY);
            leftChildOffsetX = this->prepareDrawableNode(_rbNode->left, drLeftNodeChild, _layer + 1, leftChildOffsetX, newOffsetY);
        }
        if (_rbNode->right != NIL)
        {
            drRightNodeChild = new DrawableNode(_rbNode->right, rightChildOffsetX, newOffsetY);
            rightChildOffsetX = this->prepareDrawableNode(_rbNode->right, drRightNodeChild, _layer + 1, rightChildOffsetX, newOffsetY);
        }

        parentOffsetX = leftChildOffsetX == rightChildOffsetX
                ? newOffsetX + this->offsetX * 2 // For case if right child is absent and left child has no own childs. This situation causes placing left child right below its parent.
                : (leftChildOffsetX + rightChildOffsetX) / 2;

        if (_layer == 0)
        {
            drNodeParent = new DrawableNode(_rbNode, parentOffsetX, _offsetY);
            this->root = drNodeParent;
        }
        else
        {
            drNodeParent->xPos = parentOffsetX;
            drNodeParent->yPos = _offsetY;
        }

        if (drLeftNodeChild != nullptr)
        {
            drNodeParent->left = drLeftNodeChild;
            drLeftNodeChild->parent = drNodeParent;
        }
        if (drRightNodeChild != nullptr)
        {
            drNodeParent->right = drRightNodeChild;
            drRightNodeChild->parent = drNodeParent;
        }

        this->usedXCoords[_layer]->push_back(parentOffsetX);

        return parentOffsetX;
    }

    void normalizeDrawableTree()
    {
        DrawableNode* leftCornerNode = this->root;

        while (leftCornerNode->left != nullptr)
        {
            leftCornerNode = leftCornerNode->left;
        }

        float modOffsetX = leftCornerNode->xPos * (leftCornerNode->xPos < 0 ? -1 : 1);

        this->normalizeDrawableNode(this->root, modOffsetX);
    }

    void normalizeDrawableNode(DrawableNode* drNode, float modOffsetX)
    {
        drNode->xPos += modOffsetX;

        if (drNode->left != nullptr)
        {
            this->normalizeDrawableNode(drNode->left, modOffsetX);
        }
        if (drNode->right != nullptr)
        {
            this->normalizeDrawableNode(drNode->right, modOffsetX);
        }
    }

    void removeOldGeometry()
    {
        for (auto* circle : this->circles)
        {
            delete circle;
        }
        for (auto* line : this->lines)
        {
            delete line;
        }
        for (auto* value : this->values)
        {
            delete value;
        }

        this->circles.clear();
        this->lines.clear();
        this->values.clear();
    }

    void prepareGeometry(DrawableNode* drNode)
    {
        auto* shape = new sf::CircleShape();
        shape->setFillColor(drNode->node->color == RBColor::RED ? sf::Color::Red : sf::Color::Black);
        shape->setRadius(this->circleRadius);
        shape->setPointCount(100);
        shape->setPosition(drNode->xPos, drNode->yPos);
        this->circles.push_back(shape);

        auto* value = new sf::Text();
        value->setFont(this->font);
        value->setString(std::to_string(drNode->node->value));
        value->setPosition(
                drNode->xPos + shape->getLocalBounds().width / 2 - value->getLocalBounds().width / 2,
                drNode->yPos + this->circleRadius / 2
        );
        value->setFillColor(sf::Color::White);
        value->setCharacterSize(this->fontSize);
        this->values.push_back(value);

        if (drNode->parent != nullptr)
        {
            auto* line = new sf::VertexArray(sf::LinesStrip, 2);
            (*line)[0].position = sf::Vector2f(drNode->parent->xPos + this->circleRadius, drNode->parent->yPos + this->circleRadius);
            (*line)[0].color = sf::Color::Black;
            (*line)[1].position = sf::Vector2f(drNode->xPos + this->circleRadius, drNode->yPos);
            (*line)[1].color = sf::Color::Black;
            this->lines.push_back(line);
        }

        if (drNode->left != nullptr)
        {
            this->prepareGeometry(drNode->left);
        }
        if (drNode->right != nullptr)
        {
            this->prepareGeometry(drNode->right);
        }
    }

    void openWindow()
    {
        unsigned int width, height;
        float scale = 1.3;

        width = sf::VideoMode::getDesktopMode().width / scale;
        height = sf::VideoMode::getDesktopMode().height / scale;

        sf::RenderWindow window(
                sf::VideoMode(width, height),
                "Red-Black Tree structure scheme");

        this->removeOldGeometry();
        this->prepareGeometry(this->root);

        while (window.isOpen())
        {
            sf::Event event{};
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::Resized)
                {
                    sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                    window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y)));
                }
            }

            window.clear(sf::Color::White);

            for (auto* line : this->lines)
            {
                window.draw(*line);
            }
            for (auto* circle : this->circles)
            {
                window.draw(*circle);
            }
            for (auto* value : this->values)
            {
                window.draw(*value);
            }

            window.display();
        }
    }

    void removeDrawableNode(DrawableNode* node)
    {
        if (node->left != nullptr)
        {
            this->removeDrawableNode(node->left);
        }

        if (node->right != nullptr)
        {
            this->removeDrawableNode(node->right);
        }

        if (this->root == node)
        {
            this->root = nullptr;
        }

        delete node;
    }

public:

    Drawer()
    {
        std::string fontPath;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        fontPath = "Consolas.ttf";
#elif __APPLE__
        fontPath = "/System/Library/Fonts/Keyboard.ttf";
#endif

        if (!font.loadFromFile(fontPath))
        {
            throw std::runtime_error("No font found! The app tried to find '" + fontPath + "' file with no success.");
        }
    }

    void draw(RBTree* tree)
    {
        if (this->root != nullptr)
        {
            this->removeDrawableNode(this->root);

            this->usedXCoords.clear();
        }

        this->prepareDrawableNode(tree->root);

        this->normalizeDrawableTree();

        this->openWindow();
    }
};