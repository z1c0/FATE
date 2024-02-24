#include "../../framework/include/fate.h"

class CRemoteConsole : public CFateApp
{
public:
	CRemoteConsole()
	{
		m_socket.Create();
		m_socket.SetTimeout(50);
		m_socket.Bind(5678);
	}

	virtual void Idle()
	{
		if (m_socket.Receive(m_buff, 1024) > 0)
		{
			printf("%s\n", m_buff);
		}
	}

private:
	CFUDPSocket m_socket;
	char m_buff[1024];
};

CRemoteConsole* CreateFateApp()
{
	return(new CRemoteConsole());
}
