using Campaign;

Console.Write("Hello World!\n");

var layoutPtr = LayoutAPI.campaign_layout_create(4);

try
{
    LayoutAPI.campaign_layout_push_flag(layoutPtr, "hello world!");
}
catch (Exception e)
{
    Console.Write(e.ToString());
}

LayoutAPI.campaign_layout_delete(layoutPtr);