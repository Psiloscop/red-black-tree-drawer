#include <vector>
#include <fstream>

#include "RBTree.h"
#include "Drawer.h"

int main()
{
    std::vector<unsigned int> numbers = {};
    unsigned int numberOfNodes = 25;
    unsigned int maxNumberRange = 999;
    bool readFromFile = false;
    std::string filename = "testNumbers.txt";

    // Generate numbers of read the old ones

    if (readFromFile)
    {
        std::ifstream inData;

        inData.open(filename);

        if (inData)
        {
            unsigned int number;

            do {
                inData >> number;

                numbers.push_back(number);
            }
            while (!inData.eof());

            inData.close();
        }
    }
    else
    {
        std::srand(time(nullptr));

        for (unsigned int i = 0; i < numberOfNodes; i++)
        {
            numbers.push_back(std::rand() % maxNumberRange);
        }
    }

    // Work with save function

    std::ofstream outData;

    outData.open(filename);

    if (outData)
    {
        for (unsigned int i = 0; i < numberOfNodes; i++)
        {
            outData << numbers[i] << std::endl;
        }
    }

    outData.close();

    // Work with tree

    RBTree tree;

    for (unsigned int i = 0; i < numberOfNodes; i++)
    {
        tree.insert(numbers[i]);
    }

    Drawer drawer;
    drawer.draw(&tree);

    return 0;
}