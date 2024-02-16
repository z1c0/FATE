#include "FSystemImpl.h"

//------------------------------------------------------------------------------
CFSystemImpl::CFSystemImpl(int width, int height)
{
}

//------------------------------------------------------------------------------
/* static */ void CFSystemImpl::GetPathToApplication(TCHAR *pszAppPath)
{
	char buffer[MAX_PATH];
	ssize_t length = readlink("/proc/self/exe", buffer, MAX_PATH);
	if (length != 1)
	{
		buffer[length] = 0;
		strcpy(pszAppPath, dirname(buffer));
	}
	else
	{
		pszAppPath[0] = 0;
		assert(false);
	}
}