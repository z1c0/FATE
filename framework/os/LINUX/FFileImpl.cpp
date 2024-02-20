#include "FFileImpl.h"

#include <sys/stat.h>

//------------------------------------------------------------------------------
CFFileImpl::CFFileImpl(const TCHAR* pszFileName)
{
	assert(false);
}

//------------------------------------------------------------------------------
bool CFFileImpl::IsValid() const
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
unsigned int CFFileImpl::GetSize() const
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFFileImpl::WriteBytes(unsigned char* pBytes, unsigned int count)
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFFileImpl::ReadBytes(unsigned char*& bytes, unsigned int& count)
{
	assert(false);
	return false;
}

//--------------------------------------------------------------------------------
/* static */ bool CFFileImpl::IsRoot(const TCHAR* filePath)
{
	return ::strcmp(filePath, "/") == 0;
}

//--------------------------------------------------------------------------------
/* static */ bool CFFileImpl::IsDirectory(const TCHAR* filePath)
{
	struct stat st;
  ::stat(filePath, &st);
	return S_ISDIR(st.st_mode);
}

//--------------------------------------------------------------------------------
/* static */ bool CFFileImpl::Exists(const TCHAR* filePath)
{
	struct stat st;
  return ::stat(filePath, &st) == 0;
}