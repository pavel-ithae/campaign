from os import path
import subprocess

root = path.dirname(path.dirname(path.realpath(__file__)))
csharpDirectory = "tests/csharp/CampaignCSharpTest"

print(root)

subprocess.run(["dotnet", "run", "--project", csharpDirectory], cwd=root)