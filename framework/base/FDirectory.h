#pragma once

#include "FateTypeDefs.h"

class CFDirectoryImpl;

class CFDirectory
{
public:
	CFDirectory(const TCHAR* name);
	~CFDirectory();

	const TCHAR* GetNextChild();

private:
	CFDirectoryImpl* m_pImpl;
};