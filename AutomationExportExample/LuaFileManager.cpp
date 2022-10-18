#include "stdafx.h"
#include "LuaFileManager.h"

LuaFileManager* LuaFileManager::s_Instance = nullptr;

LuaFileManager::LuaFileManager() :
	m_LuaFilePath(L""),
	m_LuaFileBytes(),
	m_LuaFileSize(0)
{
	s_Instance = this;
}
