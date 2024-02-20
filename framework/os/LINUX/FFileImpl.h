#pragma once

#include "../../base/FateTypeDefs.h"

class CFFileImpl
{
public:
	CFFileImpl(const TCHAR *pszFileName);

  bool IsValid() const;
  unsigned int GetSize() const;
  bool WriteBytes(unsigned char* pBytes, unsigned int count);
  bool ReadBytes(unsigned char*& bytes, unsigned int& count);

  static bool IsDirectory(const TCHAR* filePath);
  static bool Exists(const TCHAR* filePath);
};