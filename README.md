# luaExporter
luaExporter is a custom exporter plugin for Automation that abstracts the C++ environment and lets users easily create Lua scripts to export data

The exporter is based on a stripped down and modified version of csvExporter (https://github.com/EndfinityDev/csvExporter)

# Installation
The .dll files are to be placed in the Steam\steamapps\common\Automation\UE427\AutomationGame\Content\ExportPlugins folder

# Usage
Select luaExporter in Automation's export manager and click export
Next you will be asked to select a Lua file to run
An example script can be found in C:\Users\ [user] \AppData\Local\AutomationGame\luaExporter\ .scripts
Once complete, the exported files can be found in C:\Users\[user]\AppData\Local\AutomationGame\luaExporter\ [Folder Name], where [Folder Name] is defined in the exporter screen

# Script creation
There are several sources that can help you develop Lua scripts, such as:
- Automation's debug menu Table Explorer (Press F9 to enable the debug menu)
- csvExporter source code (https://github.com/EndfinityDev/csvExporter)
- The official exporter documentation (https://wiki.automationgame.com/index.php?title=Exporter_Plugin_SDK)
- The official Exporter SDK (https://github.com/AutomationStaff/ExporterSDK)
