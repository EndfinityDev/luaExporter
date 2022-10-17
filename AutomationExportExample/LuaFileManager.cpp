#include "stdafx.h"
#include "LuaFileManager.h"

LuaFileManager* LuaFileManager::s_Instance = nullptr;

LuaFileManager::LuaFileManager() :
	m_LuaFilePath(""),
	m_LuaFileBytes()
{
	s_Instance = this;
}
