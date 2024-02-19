#include "FDirectoryImpl.h"

//------------------------------------------------------------------------------
CFDirectoryImpl::CFDirectoryImpl(const TCHAR* name) :
  m_handle(INVALID_HANDLE_VALUE)
{
  _tcscpy(m_filePath, name);
}

//------------------------------------------------------------------------------
CFDirectoryImpl::~CFDirectoryImpl()
{
  if (m_handle != INVALID_HANDLE_VALUE)
  {
    ::FindClose(m_handle);
  }
}

//------------------------------------------------------------------------------
const TCHAR* CFDirectoryImpl::GetNextChild()
{
  if (m_handle == INVALID_HANDLE_VALUE)
  {
    m_handle = ::FindFirstFile(m_filePath, &m_ffd);
    if (m_handle == INVALID_HANDLE_VALUE)
    {
      return NULL;
    }
  }
  else
  {
    if (!::FindNextFile(m_handle, &m_ffd))
    {
      return NULL;
    }
  }
  return m_ffd.cFileName;
}
