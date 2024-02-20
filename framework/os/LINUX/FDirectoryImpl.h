#pragma once

#include "../../base/FateTypeDefs.h"

class CFDirectoryImpl
{
public:
	CFDirectoryImpl(const TCHAR* name);
	~CFDirectoryImpl();

	const TCHAR* GetNextChild();
};