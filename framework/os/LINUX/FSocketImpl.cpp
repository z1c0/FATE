#include "FSocketImpl.h"

//--------------------------------------------------------------------------------
CFSocketImpl::CFSocketImpl()
{	
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Create()
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Close()
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(int port)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(const CFInetAddrImpl* pInetAddr)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Listen()
{
	assert(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Accept(CFSocketImpl& sock)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(const CFInetAddrImpl* pInetAddr)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(char *pAddrStr, int port)
{
	assert(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Send(const char* pBuff, const int size)
{
	assert(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Receive(char* pBuff, const int size)
{
	assert(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Write(const char* pBuff, const int size)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::IsClientConnecting() const
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::IsReceiving() const
{
	assert(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::GetListenPort() const
{
	assert(false);
}

//--------------------------------------------------------------------------------
void CFSocketImpl::SetTimeout(DWORD dwTimeout)
{
	assert(false);
}

//--------------------------------------------------------------------------------
/* static */ CFInetAddrImpl* CFSocketImpl::GetHostByName(const char* pHostName, int port)
{
	assert(false);
}

//--------------------------------------------------------------------------------
/* static */ const char* CFSocketImpl::GetHostByAddr(const CFInetAddrImpl* pInetAddr)
{
	assert(false);
}

