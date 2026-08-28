from os import path
import shutil

root = path.dirname(path.dirname(path.realpath(__file__)))
binDirectory = path.join(root, "bin")
csharpLibDirectory = path.join(root, "tests/csharp/CampaignCSharpTest/libs")

soPath = path.join(binDirectory, "libcampaign.so")
dllPath = path.join(binDirectory, "libcampaign.dll")

anyCopy = False

if (path.exists(soPath)):
    shutil.copyfile(soPath, path.join(csharpLibDirectory, "libcampaign.so"))
    anyCopy = True

if (path.exists(dllPath)):
    shutil.copyfile(dllPath, path.join(csharpLibDirectory, "libcampaign.dll"))
    anyCopy = True

if (anyCopy):
    print("Library file copied to CSharp test project.")
else:
    print("Could not find a shared library file to copy.")