#import shutil
import os
import time
import zipfile
import traceback

def main():
    try:
        cwd = os.getcwd()
        if not os.path.exists(cwd + "\\Builds\\.Distribution"):
            os.makedirs(cwd + "\\Builds\\.Distribution")
        #shutil.copy2(cwd + "\\Builds\\Release\\luaExporter_x64.dll", cwd + "\\Builds\\.Distribution\\luaExporter_x64.dll")
        #shutil.copy2(cwd + "\\Builds\\Release\\luaExporter_x86.dll", cwd + "\\Builds\\.Distribution\\luaExporter_x86.dll")
        archive = zipfile.ZipFile(cwd + "\\Builds\\.Distribution\\luaExporter.zip", mode="w")

        archive.write(cwd + "\\Builds\\Release\\luaExporter_x64.dll", "luaExporter_x64.dll")
        archive.write(cwd + "\\Builds\\Release\\luaExporter_x86.dll", "luaExporter_x86.dll")
        archive.write(cwd + "\\ReadMe.txt.unpacked", "ReadMe.txt")

        archive.close()
        
        print("Packaging complete")
    except:
        traceback.print_exc()

    time.sleep(3)

if __name__ == "__main__":
    main()
