#include <campaign/api/api_layout.h>
#include <stdio.h>

int main()
{
    printf("Hello wolrd!\n");

    void *layoutPtr = campaign_layout_create(4);

    campaign_layout_delete(layoutPtr);
}