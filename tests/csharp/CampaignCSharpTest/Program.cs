using Campaign;

Console.Write("Hello World!\n");

LayoutAPI.campaign_layout_create(4, out var layoutPtr);

try
{
    LayoutAPI.campaign_layout_push_flag(layoutPtr, "hello world!");
}
catch (Exception e)
{
    Console.Write(e.ToString());
}

LayoutAPI.campaign_layout_delete(layoutPtr);

Console.Write("Success!\n");