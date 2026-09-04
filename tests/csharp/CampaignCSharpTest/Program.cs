using Campaign.API;

Console.Write("Hello World!\n");

LayoutAPI.Create(4, out var layoutPtr);

try
{
    LayoutAPI.EntryExists(layoutPtr, "hello world!", out bool exists);
    Console.WriteLine(exists);

    LayoutAPI.PushFlag(layoutPtr, "hello world!");

    LayoutAPI.EntryExists(layoutPtr, "hello world!", out exists);
    Console.WriteLine(exists);
}
catch (Exception e)
{
    Console.Write(e.ToString());
}

LayoutAPI.Delete(layoutPtr);

Console.Write("Success!\n");