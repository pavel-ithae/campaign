#include <campaign/api/api_layout.h>
#include <campaign/layout.hpp>
#include <iostream>

using namespace campaign;

void *campaign_layout_create(int32_t descriptorCount)
{
    Layout *layoutPtr = new Layout(descriptorCount);

    std::cout << "Layout Created!!\n";

    return (void *)layoutPtr;
}

void campaign_layout_delete(void *layoutPtr)
{
    delete (Layout *)layoutPtr;

    std::cout << "Layout Deleted!\n";
}