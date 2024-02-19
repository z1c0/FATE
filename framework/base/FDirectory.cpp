#include "FDirectory.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
	#include "../os/WIN32/FDirectoryImpl.h"
#elif defined (_LINUX)
	#include "../os/LINUX/FDirectoryImpl.h"
#elif defined (_PALM_OS)
	//#include "../os/PALM_OS/FDirectoryImpl.h"
#else
	#error
#endif


//--------------------------------------------------------------------------------
CFDirectory::CFDirectory(const TCHAR* name) : m_pImpl(new CFDirectoryImpl(name))
{
}

//--------------------------------------------------------------------------------
CFDirectory::~CFDirectory()
{
	delete m_pImpl;
}

//--------------------------------------------------------------------------------
const TCHAR* CFDirectory::GetNextChild()
{
	return m_pImpl->GetNextChild();
}