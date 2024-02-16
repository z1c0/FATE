#include "FFile.h"

//------------------------------------------------------------------------------
CFFile::CFFile()
{
  m_handle = INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
CFFile::CFFile(TCHAR* pszFileName)
{
  m_handle = CreateFile(pszFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    
}

//------------------------------------------------------------------------------
CFFile::~CFFile()
{
  if (m_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(m_handle);
  }
}

//------------------------------------------------------------------------------
const bool CFFile::IsValid()
{
  return (m_handle != INVALID_HANDLE_VALUE);
}

//------------------------------------------------------------------------------
const unsigned int CFFile::GetSize()
{
  int ret = 0;
  if (m_handle != INVALID_HANDLE_VALUE) {
    ret = GetFileSize(m_handle, NULL);
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CFFile::WriteBytes(unsigned char* pBytes, unsigned int count)
{
  bool ret = false;
  if (m_handle != INVALID_HANDLE_VALUE) {
    DWORD dwWritten = 0;
    if (WriteFile(m_handle, pBytes, count, &dwWritten, NULL)) {
      ret = (count == dwWritten);
    }    
  }
  return ret;
}

//------------------------------------------------------------------------------
const bool CFFile::ReadBytes(unsigned char*& bytes, unsigned int& count)
{
  bool ret = false;
  if ((m_handle != INVALID_HANDLE_VALUE)&&(count > 0)) {
    DWORD dwBytesRead = 0;
    unsigned char *tempbytes = new unsigned char[count];
    if (::ReadFile(m_handle, tempbytes, count, &dwBytesRead, NULL) != 0) {      
      if (count == dwBytesRead) {
        count = dwBytesRead;
        bytes = tempbytes;
        ret = true;
      }
    } 
    if (!ret) {
      if (tempbytes) {
        delete tempbytes;
      }
    }
  }
  return ret;
}
