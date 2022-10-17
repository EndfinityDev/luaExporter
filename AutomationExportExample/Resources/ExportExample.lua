---------------------------------------------------
-- Copyright (c) 2021 -- Camshaft Software PTY LTD
---------------------------------------------------

function DoExport(CarCalculator, CarFile)
	UAPI.Log("DoExport: ")

	local CarFiles = CExporter.ExportCarFiles(CarCalculator)
	CarFiles[string.format("%s - %s.car", CarCalculator.CarInfo.PlatformInfo.Name, CarCalculator.CarInfo.TrimInfo.Name)] = CarFile

	local Data = {}

	local Files = {}

	for k,v in pairs(CarFiles) do
		Files[k] = v
	end

	return Files, Data
end



if CExporter == nil then
	CExporter = {}
	CExporter.__index = CExporter
end


function CExporter.ExportCarFiles(CarCalculator)
	local CarInfo = CarCalculator.CarInfo

	Files = { }

	Files["carInfoDump.txt"] = dump(CarInfo)

	return Files
end