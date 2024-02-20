#pragma once

#include "FateTypeDefs.h"

class CFFileImpl;

class CFFile
{
public:
  CFFile(const TCHAR *pszFileName);
  virtual ~CFFile();

  bool IsValid() const;
  unsigned int GetSize() const;
  bool WriteBytes(unsigned char* pBytes, unsigned int count);
  bool ReadBytes(unsigned char*& bytes, unsigned int& count);

  static bool IsDirectory(const TCHAR* filePath);
  static bool Exists(const TCHAR* filePath);

private:
  CFFileImpl* m_pImpl;
};
