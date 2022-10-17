/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#pragma once

#include "luaExporter.h"
#include "EXPVERSION.h"

class AuExpManager
{
public:

	static AuExpManager* Instance() { return s_Instance; }
	static void CreateInstance() { if (!s_Instance) s_Instance = new AuExpManager(); }
	static void DestroyInstance() { delete s_Instance; s_Instance = nullptr; }


	AuCarExpErrorCode Init(const AuCarExpCarData* carData);

	bool IsExportInProcess() { return m_IsExportInProcess; }

	//Getting pixel data is a threaded operation within Automation, which means that we should not assume pixel data within these textures is complete until EndExport() is called
	//So what we do is save the reference and save to file at the end of the export
	void EndExport();


	const wchar_t* GetExportDirectory() const { return m_ExportDirectory.c_str(); }


	void AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& Data);

	const wchar_t* GetCarName() const { return m_CarData->GetCarName(); }

	const std::wstring IllegalCharacters = L"\\/:?\"<>|";

private:

	AuExpManager();
	~AuExpManager() {}

	void ExportInternal();

	size_t FindDirDelimiter(std::wstring dir, size_t start);

	static AuExpManager* s_Instance;

	bool m_IsExportInProcess = false;

	const AuCarExpCarData* m_CarData;

	std::wstring m_ExportDirectory;
	//std::wstring m_ExportFolder;


	std::map<std::wstring, float> m_LuaFloatData;
	std::map<std::wstring, std::wstring> m_LuaStringData;
	std::map<std::wstring, AuCarLuaDataFile> m_LuaFDataFiles;

	std::wstring m_FileName = L"";
	std::wstring m_EngineName = L"";

};



