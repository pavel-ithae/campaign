import os;
from os import path;
import shutil

root = path.dirname(path.dirname(path.realpath(__file__)))
binDirectory = path.join(root, "bin")
csharpLibDirectory = path.join(root, "tests/csharp/CampaignCSharpTest/libs")

soPath = path.join(binDirectory, "libcampaign.so")
soPathExists = path.exists(soPath)

dllPath = path.join(binDirectory, "libcampaign.dll")
dllPathExists = path.exists(dllPath)

if (not (soPathExists or dllPathExists)):
    print("Could not find a shared library file to copy.")
    exit()

os.makedirs(csharpLibDirectory, exist_ok=True)

if (soPathExists):
    shutil.copyfile(soPath, path.join(csharpLibDirectory, "libcampaign.so"))

if (dllPathExists):
    shutil.copyfile(dllPath, path.join(csharpLibDirectory, "libcampaign.dll"))

print("Library file copied to CSharp test project.") 