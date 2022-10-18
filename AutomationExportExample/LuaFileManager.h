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
	LPCWSTR GetLuaFilePath() { return m_LuaFilePath; }
	void SetLuaFilePath(LPCWSTR path) { m_LuaFilePath = path; }
	std::vector<byte> GetLuaFileBytes() { return m_LuaFileBytes; }
	void SetLuaFileBytes(std::vector<byte> buffer) { m_LuaFileBytes = buffer; }
	void SetLuaFileSize(size_t size) { m_LuaFileSize = size; }
	size_t GetLuaFileSize() { return m_LuaFileSize; }
private:
	LuaFileManager();
	~LuaFileManager() {}

	static LuaFileManager* s_Instance;
	LPCWSTR m_LuaFilePath;
	std::vector<byte> m_LuaFileBytes;
	size_t m_LuaFileSize;
};

