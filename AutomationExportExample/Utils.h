#pragma once
#include <string>

static class Utils
{
public:
	static size_t Utils::FindDirDelimiter(std::wstring dir, size_t start)
	{
		size_t slashPos = dir.find(L"\\", start);

		if (slashPos == std::wstring::npos)
		{
			return dir.find(L"/", start);
		}

		return slashPos;
	}
};