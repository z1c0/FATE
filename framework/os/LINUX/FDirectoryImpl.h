#pragma once

#include "../../base/FateTypeDefs.h"
#include <dirent.h>

class CFDirectoryImpl
{
public:
	CFDirectoryImpl(const TCHAR* name);
	~CFDirectoryImpl();

	const TCHAR* GetNextChild();

private:
  TCHAR m_filePath[MAX_PATH];
	DIR *m_pDir;
};