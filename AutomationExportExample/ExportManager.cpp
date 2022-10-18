/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#include "stdafx.h"
#include <Shlobj.h>
#include "FileManager.h"
#include "LuaFileManager.h"
#include <shobjidl.h>
#include <commdlg.h>
#include <fstream>

size_t AuExpManager::FindDirDelimiter(std::wstring dir, size_t start)
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
	m_ExportDirectory(L"")
	//m_ExportFolder(L"luaExporter")
{
	s_Instance = this;
}


AuCarExpErrorCode AuExpManager::Init(const AuCarExpCarData* carData)
{
	TCHAR path[MAX_PATH];

	m_CarData = carData;
	//m_ExportFolder = m_CarData->GetStringData(1)->Value;

	//get the user's documents directory:
	if (SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_CURRENT, path) == S_OK)
	{
		m_ExportDirectory = path;
		m_ExportDirectory += L"\\AutomationGame\\luaExporter\\";
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

	AuCarExpErrorCode errCode = InitLuaFileManager();

	if (errCode != AuCarExpErrorCode_Success)
		return errCode;

}

AuCarExpErrorCode AuExpManager::InitLuaFileManager()
{
	LuaFileManager::Init();

	HGLOBAL hResourceLoaded;  // handle to loaded resource
	HRSRC   hRes;              // handle/ptr to res. info.

	HMODULE module = GetModuleHandle(PROJECT_FILENAME);

	hRes = FindResource(module, MAKEINTRESOURCE(IDR_LUA_FILE), TEXT("BINARY"));

	TCHAR path[MAX_PATH];

	if (hRes)
	{

		unsigned int size = SizeofResource(module, hRes);

		hResourceLoaded = LoadResource(module, hRes);
		char* data = (char*)LockResource(hResourceLoaded);


		std::wstring exampleLuaFilePath;

		//get the user's documents directory:
		if (SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_CURRENT, path) == S_OK)
		{
			exampleLuaFilePath = path;
			exampleLuaFilePath += L"\\AutomationGame\\luaExporter\\.scripts\\";

			DWORD att = GetFileAttributes(exampleLuaFilePath.c_str());

			if (att == INVALID_FILE_ATTRIBUTES)
			{
				//create directory, one level at a time:
				size_t slashPos = FindDirDelimiter(exampleLuaFilePath, 0);
				size_t offset = 0;

				while (slashPos != std::wstring::npos)
				{
					CreateDirectory(exampleLuaFilePath.substr(offset, slashPos - offset).c_str(), nullptr);
					slashPos = FindDirDelimiter(exampleLuaFilePath, slashPos + 1);
				}

				//last one:
				CreateDirectory(exampleLuaFilePath.c_str(), nullptr);

				att = GetFileAttributes(exampleLuaFilePath.c_str());
			}

			if (att != INVALID_FILE_ATTRIBUTES && att & FILE_ATTRIBUTE_DIRECTORY)
			{
				FileManager fileManager;

				unsigned int dataSize = SizeofResource(module, hRes);

				FILE* OutFile = fileManager.OpenFileGlobal((exampleLuaFilePath + L"ExportExample.lua").c_str(), L"wb");
				if (OutFile)
					fwrite(data, 1, dataSize, OutFile);
			}
		}
		UnlockResource(hResourceLoaded);
	}

	OPENFILENAMEW ofn;
	WCHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	LPCWSTR filter = L"Lua Files (*.lua)\0*.lua\0All Files (*.*)\0*.*\0";
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	wchar_t initDir[MAX_PATH];
	std::wstring localDir;
	std::wstring fullDir;

	if (SHGetFolderPathW(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_CURRENT, initDir) == S_OK)
	{
		localDir = L"\\AutomationGame\\luaExporter\\.scripts\\";
		fullDir = initDir + localDir;
		ofn.lpstrInitialDir = fullDir.c_str();
	}
	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		//s_LuaFilePath = ofn.lpstrFile;
		LuaFileManager::Instance()->SetLuaFilePath(ofn.lpstrFile);
	}
	else
	{
		return AuCarExpErrorCode_UnknownError;
	}


	std::ifstream file(LuaFileManager::Instance()->GetLuaFilePath(), std::ifstream::ate | std::ifstream::binary);

	if (!file.is_open())
		return AuCarExpErrorCode_UnknownError;

	// get the length of the file
	file.seekg(0, std::ifstream::end);
	size_t fileSize = file.tellg();
	file.seekg(0, std::ifstream::beg);

	// create a vector to hold all the bytes in the file
	std::vector<byte> data(fileSize, 0);

	// read the file
	file.read(reinterpret_cast<char*>(&data[0]), fileSize);
	LuaFileManager::Instance()->SetLuaFileBytes(data);

	// close the file
	file.close();

	LuaFileManager::Instance()->SetLuaFileSize(fileSize);

	return AuCarExpErrorCode_Success;
}

void AuExpManager::EndExport() 
{
	m_IsExportInProcess = true;
	ExportInternal();

	m_IsExportInProcess = false;
}

void AuExpManager::ExportInternal()
{
	
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


