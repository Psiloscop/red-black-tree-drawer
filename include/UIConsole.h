#pragma once

#include <iostream>

#include "RBTree.h"
#include "Drawer.h"

class UIConsole
{
protected:

    RBTree* tree;

    Drawer* drawer;

    std::string menuText;

public:

    UIConsole(RBTree* _tree, Drawer* _drawer): tree{_tree}, drawer{_drawer}
    {
        this->menuText = "Red-Black tree manipulation.\nSelect action by its number:";
        this->menuText += "\n1. Insert\n2. Delete\n3. Clear tree\n4. Display tree\n5. Exit";
        this->menuText += "\n\nAction: ";
    }

    void run()
    {
        unsigned int actionNumber;
        bool exitConfirmed = false;

        do {
            system("cls");

            std::cout << this->menuText;

            std::cin >> actionNumber;

            switch (actionNumber)
            {
                case 1:
                    this->firstActionHandler();
                    break;
                case 2:
                    this->secondActionHandler();
                    break;
                case 3:
                    this->thirdActionHandler();
                    break;
                case 4:
                    this->fourthActionHandler();
                    break;
                case 5:
                    exitConfirmed = true;
                    break;
                default:
                    break;
            }
        }
        while (!exitConfirmed);
    }

    /**
     * Insert action handler
     */
    void firstActionHandler()
    {
        int value;

        std::cout << "Enter integer value to insert: ";
        std::cin >> value;

        this->tree->insert(value);
    }

    /**
     * Delete action handler
     */
    void secondActionHandler()
    {
        int value;

        std::cout << "Enter integer value to delete: ";
        std::cin >> value;

        this->tree->remove(value);
    }

    /**
     * Clear action handler
     */
    void thirdActionHandler()
    {
        this->tree->clear();
    }

    /**
     * Display action handler
     */
    void fourthActionHandler()
    {
        if (this->tree->root != NIL)
        {
            this->drawer->draw(this->tree);
        }
        else
        {
            std::cout << "The tree is empty.\n";
        }
    }
};