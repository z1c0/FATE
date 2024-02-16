#pragma once

#include "../FateTypeDefs.h"

class CFFile
{

public:

  CFFile();
  CFFile(TCHAR *pszFileName);
  virtual ~CFFile();

  const bool IsValid();
  const unsigned int GetSize();
  bool WriteBytes(unsigned char* pBytes, unsigned int count);
  const bool ReadBytes(unsigned char*& bytes, unsigned int& count);

protected:
  HANDLE m_handle;
};
