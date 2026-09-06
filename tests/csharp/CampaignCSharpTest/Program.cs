using Campaign;

Console.Write("Hello World!\n");

void Call()
{
    Layout? layout = new Layout(4);

    try
    {
        Console.WriteLine(layout.EntryExist("hello world!"));

        layout.PushFlag("my_flag!");

        var entryInfo = layout.GetEntryInfo(0);

        Console.WriteLine(entryInfo.id);

        Console.WriteLine(layout.GetFlagInfo("my_flag!"));
    }
    catch (Exception e)
    {
        Console.Write(e.ToString());
    }

    layout = null;
}

Call();

GC.Collect(2);
GC.WaitForPendingFinalizers();

Console.Write("Success!\n");
