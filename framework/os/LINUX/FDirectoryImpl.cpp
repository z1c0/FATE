#include "FDirectoryImpl.h"

//--------------------------------------------------------------------------------
CFDirectoryImpl::CFDirectoryImpl(const TCHAR* name) : m_pDir(NULL)
{
	_tcscpy(m_filePath, name);
}

//--------------------------------------------------------------------------------
CFDirectoryImpl::~CFDirectoryImpl()
{
	if (m_pDir)
	{
		::closedir(m_pDir);
	}
}

//--------------------------------------------------------------------------------
const TCHAR* CFDirectoryImpl::GetNextChild()
{
	if (!m_pDir)
	{
		m_pDir = ::opendir(m_filePath);
		if (!m_pDir)
		{
			return NULL;
		}
	}
	dirent* entry = ::readdir(m_pDir);
	return entry ? entry->d_name : NULL;
}