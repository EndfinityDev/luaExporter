#pragma once
#include "stdafx.h"
#include <shobjidl.h>
#include <ShlObj.h>

class LuaFileManager
{
public:
	static LuaFileManager* Instance() { return s_Instance; }

	static void Init() { if (!s_Instance) s_Instance = new LuaFileManager(); }
	static void Free() { s_Instance->m_LuaFileBytes.clear(); delete s_Instance; s_Instance = nullptr; }
	LPCSTR GetLuaFilePath() { return m_LuaFilePath; }
	void SetLuaFilePath(LPCSTR path) { m_LuaFilePath = path; }
	std::vector<byte> GetLuaFileBytes() { return m_LuaFileBytes; }
	void SetLuaFileBytes(std::vector<byte> buffer) { m_LuaFileBytes = buffer; }
private:
	LuaFileManager();
	~LuaFileManager() {}

	static LuaFileManager* s_Instance;
	LPCSTR m_LuaFilePath;
	std::vector<byte> m_LuaFileBytes;
};

