#include "FFileImpl.h"

#include <sys/stat.h>
//--------------------------------------------------------------------------------
/* static */ bool CFFileImpl::IsDirectory(const TCHAR* filePath)
{
	assert(false);
}

//--------------------------------------------------------------------------------
/* static */ bool CFFileImpl::Exists(const TCHAR* filePath)
{
	struct stat st;
  return ::stat(filePath, &st) == 0;
}