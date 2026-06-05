#include "header.hpp"

std::vector<int> bubblesort(std::vector<int> array)
{
    for (size_t i = 1; i < array.size(); i++)
    {
        if (array[i - 1] > array[i])
        {
            array[i - 1] += array[1];
            //array[1] +=
        }
    }
}
