#pragma once

#include "../../base/FateTypeDefs.h"

class CFDirectoryImpl
{
public:
  CFDirectoryImpl(const TCHAR* name);
  ~CFDirectoryImpl();

  const TCHAR* GetNextChild();

private:
  TCHAR m_filePath[MAX_PATH];
  HANDLE m_handle;
  WIN32_FIND_DATA m_ffd;
};
