using System;
using System.Runtime.InteropServices;

Console.Write("Hello World!\n");
Console.Write("Done!\n");

var layoutPtr = campaign_layout_create(4);

campaign_layout_delete(layoutPtr);

[DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
static extern IntPtr campaign_layout_create(int descriptorCount);

[DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
static extern void campaign_layout_delete(IntPtr layoutPtr);