/************************************************* *
** Copyright(c) 2019 -- Camshaft Software PTY LTD
** All Rights Reserved
************************************************** */

#include "stdafx.h"

#include "..\Include\AuCarExportDLL.h"

#include "EXPVERSION.h"

#include <windows.h>
#include <shobjidl.h>
#include <commdlg.h>
#include <fstream>
#include <ShlObj.h>
#include "FileManager.h"
#include "LuaFileManager.h"

//static LPCSTR s_LuaFilePath;
//static std::vector<byte> s_LuaFileBytes;

//Gets the plugin name, to be displayed in the drop-down list of available plugins
AuCarExpErrorCode AuCarExportDLL::GetExporterName(AuCarExpArray<wchar_t>& name, wchar_t const* locale)
{
	wcscpy_s(name.GetData(), name.GetCount(), L"luaExporter");

	return AuCarExpErrorCode_Success;
}

//Gets the plugin version number
//This is for display in the UI only (to help when users report bugs, etc.). Automation does nothing else with this information.
AuCarExpErrorCode AuCarExportDLL::GetExporterVersion(unsigned int* VersionNumber)
{
	*VersionNumber = EXPVERSION;

	return AuCarExpErrorCode_Success;
}

//Gets the number of user-supplied strings that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringDataCount(unsigned int* retCount)
{
	*retCount = 1;

	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied string information
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringData(AuCarExpArray<AuCarExpUIStringData>& stringData, wchar_t const* locale)
{
	if (stringData.GetCount() != 1)
	{
		//Automation has not given us the number of items we asked for
		//(this should never happen)
		return AuCarExpErrorCode_UnknownError;
	}

	//set the values:
	wcscpy_s(stringData[0].Label, L"Folder Name");//label
	wcscpy_s(stringData[0].Value, L"[PlayerName] [PlatformName] [TrimName]");//default value

	return AuCarExpErrorCode_Success;
}

//Gets the number of user-supplied booleans that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolDataCount(unsigned int* retCount)
{
	*retCount = 0;

	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied boolean information
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData, wchar_t const* locale)
{
	if (boolData.GetCount() != 0)
	{
		//Automation has not given us the number of items we asked for
		//(this should never happen)
		return AuCarExpErrorCode_UnknownError;
	}


	return AuCarExpErrorCode_Success;
}


AuCarExpErrorCode AuCarExportDLL::BeginExport(const AuCarExpCarData* carData, AuCarExpArray<wchar_t>& retDir, unsigned int* retFlags)
{
	AuExpManager::CreateInstance();
	AuCarExpErrorCode error = AuExpManager::Instance()->Init(carData);

	//tell Automation which directory the plugin will be exporting files to (so this can be displayed in a message to the user):
	wcscpy_s(retDir.GetData(), retDir.GetCount(), AuExpManager::Instance()->GetExportDirectory());

	//set the flags to none:
	*retFlags = AuCarExpExporterFlags_None;

	return error;
}

//Called after all data has been supplied to the export plugin, and all texture information has been finalised
AuCarExpErrorCode AuCarExportDLL::EndExport()
{
	AuExpManager::Instance()->EndExport();

	return AuCarExpErrorCode_Success;
}

//Called after EndExport(), to give the plugin the chance to finish any threaded operations that may still be running
AuCarExpErrorCode AuCarExportDLL::IsExportInProgress(bool* retInProgress)
{
	//*retInProgress = false;//set to true if there are still threads running
	*retInProgress = AuExpManager::Instance()->IsExportInProcess();

	return AuCarExpErrorCode_Success;
}

//Called after the export has finished, and IsExportInProgress() has reported nothing in progress.
//There will be no further function calls for this export.
//All outstanding plugin-allocated memory should be cleaned up here
AuCarExpErrorCode AuCarExportDLL::FreeAllData()
{
	//de-allocate everything:
	AuExpManager::DestroyInstance();

	return AuCarExpErrorCode_Success;
}


//Export the car body mesh
AuCarExpErrorCode AuCarExportDLL::AddBodyMesh(AuCarExpMesh* mesh)
{
	return AuCarExpErrorCode_Success;
}

//Export a (single) fixture
AuCarExpErrorCode AuCarExportDLL::AddFixtureMeshes(const AuCarExpArray<AuCarExpMesh*>& meshes, bool isBreakable, const wchar_t* name, const bool isTowBar, const AuCarExpVector& towPosition)
{
	return AuCarExpErrorCode_Success;
}

//Export an engine mesh
AuCarExpErrorCode  AuCarExportDLL::AddEngineMesh(const AuCarExpMesh* mesh, const wchar_t* name)
{
	return AuCarExpErrorCode_Success;
}

//Export an engine mesh whose mesh data is shared with another mesh
AuCarExpErrorCode  AuCarExportDLL::AddEngineMeshInstance(const AuCarExpMeshInstance* meshInstance)
{
	return AuCarExpErrorCode_Success;
}

//Export an exhaust line
//meshes will include piping, catalytic converter, mufflers and exhaust tip fixture (if present)
AuCarExpErrorCode  AuCarExportDLL::AddExhaust(const AuCarExpArray<AuCarExpMesh*>& meshes, const AuCarExpArray<AuCarExpVector>& points)
{
	return AuCarExpErrorCode_Success;
}

//Set the driver and bonnet camera positions
AuCarExpErrorCode  AuCarExportDLL::AddCameraPositions(const AuCarExpVector* driverCamPosition, const AuCarExpVector* bonnetCamPosition)
{
	return AuCarExpErrorCode_Success;
}

//Export a pair of wheels (either front or back), including the suspension meshes
AuCarExpErrorCode  AuCarExportDLL::AddWheelPair(const AuCarExpWheelData& wheelData, const bool isFront)
{
	return AuCarExpErrorCode_Success;
}

//Export the chassis meshes
AuCarExpErrorCode  AuCarExportDLL::AddChassis(const AuCarExpArray<AuCarExpMesh*>& meshes)
{
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::SetStampTexture(AuCarExpTexture* stampTexture)
{
	return AuCarExpErrorCode_Success;
}




//Get render information required to generate a preview image
AuCarExpErrorCode AuCarExportDLL::GetPreviewImageData(unsigned int* retTextureWidth, unsigned int* retTextureHeight, float* retFOV)
{
	*retTextureWidth = 500;//required image width in pixels
	*retTextureHeight = 281;//required image height in pixels
	*retFOV = 17.0f;//required field of view

	return AuCarExpErrorCode_Success;
}

//Get camera transform information required to generate a preview image
AuCarExpErrorCode AuCarExportDLL::GetPreviewTransformData(const AuCarExpVector* carMin, const AuCarExpVector* carMax, AuCarExpVector* retPosition, AuCarExpVector* retLookAt)
{
	float carLength = carMax->z - carMin->z;
	float carHeight = carMax->y - carMin->y;

	retLookAt->x = (carMin->x + carMax->x) * 0.0f;
	retLookAt->y = (carMin->y + carMax->y) * 0.5f;
	retLookAt->z = (carMin->z + carMax->z) * 0.5f;

	retLookAt->y -= 0.02f * carHeight;

	*retPosition = *retLookAt;

	retPosition->x -= 3.5f * carLength;

	return AuCarExpErrorCode_Success;
}

//Export the generated preview image for the car
AuCarExpErrorCode AuCarExportDLL::AddPreviewImage(AuCarExpTexture* image)
{
	return AuCarExpErrorCode_Success;
}


AuCarExpErrorCode AuCarExportDLL::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& Data)
{
	AuExpManager::Instance()->AddLuaFiles(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data)
{
	//AuExpManager::Instance()->AddLuaFloatData(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data)
{
	//AuExpManager::Instance()->AddLuaStringData(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLUAFileLength(unsigned int* retLength)
{
	
	unsigned int size = LuaFileManager::Instance()->GetLuaFileSize();

	*retLength = size; //size in chars (what we need) is the byte size. We add one for a null terminator

	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLUAFile(AuCarExpArray<wchar_t>& stringBuffer)
{
	if (!stringBuffer.GetData())
	{
		return AuCarExpErrorCode_UnknownError;
	}

	std::vector<byte> data = LuaFileManager::Instance()->GetLuaFileBytes();

	unsigned int fileSize = stringBuffer.GetCount() - 1;
	for (unsigned int i = 0; i < fileSize; i++)
	{
		stringBuffer[i] = data[i];
	}

	stringBuffer[fileSize] = '\0';

	data.clear();
	LuaFileManager::Free();
	//delete &s_LuaFileBytes;
	
	//delete &s_LuaFilePath;

	return AuCarExpErrorCode_Success;
}

