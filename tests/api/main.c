#include <unity.h>
#include <stdio.h>

#include "test_api_layout.h"

#include <campaign/api/api_except.h>
#include <campaign/api/api_string.h>

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

    campaign_layout_push_flag(NULL, "test");

    return UNITY_END();
}