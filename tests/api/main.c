#include "test_api_layout.h"
#include "test_api_blueprint.h"

#include <unity.h>

void setUp(void) 
{
}

void tearDown(void)
{
}

int main()
{
    UNITY_BEGIN();

    test_campaign_layout();
    test_campaign_blueprint();

    return UNITY_END();
}