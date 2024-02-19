#include "FFile.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
  #include "../os/WIN32/FFileImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FFileImpl.h"
#elif defined (_PALM_OS)
  //#include "../os/PALM_OS/FFileImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFFile::CFFile(const TCHAR *pszFileName) : m_pImpl(new CFFileImpl(pszFileName))
{
}

//------------------------------------------------------------------------------
CFFile::~CFFile()
{
  delete m_pImpl;
}

//------------------------------------------------------------------------------
bool CFFile::IsValid() const
{
  return m_pImpl->IsValid();
}

//------------------------------------------------------------------------------
unsigned int CFFile::GetSize() const
{
  return m_pImpl->GetSize();
}

//------------------------------------------------------------------------------
bool CFFile::ReadBytes(unsigned char*& bytes, unsigned int& count)
{
  return m_pImpl->ReadBytes(bytes, count);
}

//------------------------------------------------------------------------------
bool CFFile::WriteBytes(unsigned char* pBytes, unsigned int count)
{
  return m_pImpl->WriteBytes(pBytes, count);
}

//------------------------------------------------------------------------------
/* static */ bool CFFile::IsDirectory(const TCHAR* fileName)
{
  return CFFileImpl::IsDirectory(fileName);
}