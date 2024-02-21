#include "FFileImpl.h"

//------------------------------------------------------------------------------
CFFileImpl::CFFileImpl(const TCHAR* pszFileName)
{
  m_handle = CreateFile(pszFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
}

//------------------------------------------------------------------------------
CFFileImpl::~CFFileImpl()
{
  if (m_handle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(m_handle);
  }
}

//------------------------------------------------------------------------------
bool CFFileImpl::IsValid() const
{
  return m_handle != INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
unsigned int CFFileImpl::GetSize() const
{
  int ret = 0;
  if (m_handle != INVALID_HANDLE_VALUE) {
    ret = GetFileSize(m_handle, NULL);
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CFFileImpl::WriteBytes(unsigned char* pBytes, unsigned int count)
{
  bool ret = false;
  if (m_handle != INVALID_HANDLE_VALUE)
  {
    DWORD dwWritten = 0;
    if (WriteFile(m_handle, pBytes, count, &dwWritten, NULL))
    {
      ret = (count == dwWritten);
    }    
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CFFileImpl::ReadBytes(unsigned char*& bytes, unsigned int& count)
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

//------------------------------------------------------------------------------
/* static */ bool CFFileImpl::IsDirectory(const TCHAR* fileName)
{
  DWORD attrib = ::GetFileAttributes(fileName);
  return (attrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

//------------------------------------------------------------------------------
/* static */ bool CFFileImpl::IsRoot(const TCHAR* fileName)
{
  return _tcslen(fileName) <= 3;
}


