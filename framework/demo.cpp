#include "include/fate.h"

class CDemo : public CFateApp
{
public:
	CDemo() : m_btnQuit("Quit"), m_btnAction("Action") {}

	virtual bool InitFateApp()
	{
		m_btnQuit.SetId(1234);
		Add(m_btnQuit);

		m_btnAction.SetId(2345);
		m_btnAction.SetY(m_btnQuit.GetBottom() + 20);
		Add(m_btnAction);

		return true;
	}

private:
	CFButton m_btnQuit;
	CFButton m_btnAction;
};

extern CFateApp* CreateFateApp()
{
	return new CDemo();
}
