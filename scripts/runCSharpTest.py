from os import path
import subprocess

root = path.dirname(path.dirname(path.realpath(__file__)))
csharpDirectory = path.join(root, "tests/csharp/CampaignTests")

print(root)

subprocess.run(["dotnet", "test"], cwd=csharpDirectory)