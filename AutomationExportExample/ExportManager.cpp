/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#include "stdafx.h"
#include <Shlobj.h>
#include "FileManager.h"

size_t FindDirDelimiter(std::wstring dir, size_t start)
{
	size_t slashPos = dir.find(L"\\", start);

	if (slashPos == std::wstring::npos)
	{
		return dir.find(L"/", start);
	}

	return slashPos;
}

AuExpManager* AuExpManager::s_Instance = nullptr;

AuExpManager::AuExpManager() :
	m_ExportDirectory(L""),
	m_ExportFolder(L"luaExporter")
{
	s_Instance = this;
}


AuCarExpErrorCode AuExpManager::Init(const AuCarExpCarData* carData)
{
	TCHAR path[MAX_PATH];

	m_CarData = carData;
	m_ExportFolder = m_CarData->GetStringData(1)->Value;

	//get the user's documents directory:
	if (SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_CURRENT, path) == S_OK)
	{
		m_ExportDirectory = path;
		m_ExportDirectory += L"\\AutomationGame\\luaExporter\\" + m_ExportDirectory;
	}
	else
	{
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}


	m_ExportDirectory += L"\\";
	//m_ExportDirectory += carData->GetCarName();//TODO: sanitise filename

	// Sanitizing Filename
	std::wstring exportFileName = m_CarData->GetStringData(0)->Value;

	for (std::wstring::iterator i = exportFileName.begin(); i < exportFileName.end(); i++)
	{
		if (IllegalCharacters.find(*i) != std::wstring::npos)
		{
			*i = L'_';
		}
	}

	//m_ExportDirectory += m_CarData->GetStringData(0)->Value;
	m_ExportDirectory += exportFileName;

	//ensure target directory exists:
	DWORD att = GetFileAttributes(m_ExportDirectory.c_str());

	if (att == INVALID_FILE_ATTRIBUTES)
	{
		//create directory, one level at a time:
		size_t slashPos = FindDirDelimiter(m_ExportDirectory, 0);
		size_t offset = 0;

		while (slashPos != std::wstring::npos)
		{
			CreateDirectory(m_ExportDirectory.substr(offset, slashPos - offset).c_str(), nullptr);
			slashPos = FindDirDelimiter(m_ExportDirectory, slashPos + 1);
		}

		//last one:
		CreateDirectory(m_ExportDirectory.c_str(), nullptr);

		att = GetFileAttributes(m_ExportDirectory.c_str());
	}

	if (att != INVALID_FILE_ATTRIBUTES && att & FILE_ATTRIBUTE_DIRECTORY)
	{
		//directory exists, all good to go:
		return AuCarExpErrorCode_Success;
	}
	else
	{
		std::wstring error = L"Could not create directory: ";
		error += m_ExportDirectory;

		MessageBox(nullptr, error.c_str(), TEXT("Error creating directory"), MB_OK);

		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}

}

void AuExpManager::EndExport() 
{
	m_IsExportInProcess = true;
	ExportInternal();

	m_IsExportInProcess = false;
}

void AuExpManager::ExportInternal()
{
	bool isEngineSeparate = m_CarData->GetBoolData(0)->Value;
	bool areChoicesSeparate = m_CarData->GetBoolData(1)->Value;

	m_FileName = m_CarData->GetStringData(0)->Value;

	for (std::wstring::iterator i = m_FileName.begin(); i < m_FileName.end(); i++)
	{
		if (IllegalCharacters.find(*i) != std::wstring::npos)
		{
			*i = L'_';
		}
	}
	
	//return true;
}

void AuExpManager::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& data)
{
	FileManager fileManager;

	for (int i = 0; i < data.GetCount(); i++)
	{
		m_LuaFDataFiles[data[i].FileName] = data[i];

		AuCarLuaDataFile& File = data[i];

		std::wstring FileName = File.FileName;
		//FileName = TextUtils::SanitiseFileName(FileName);
		//FILE* OutFile; //= m_FileManager->OpenFile(m_Directory + L"/" + FileName, L"wb");
		FILE* OutFile = fileManager.OpenFile((FileName).c_str(), L"wb");
		if (!OutFile)
		{
			return;
		}

		fwrite(File.Buffer, 1, File.BufferSize, OutFile);

	}
}


