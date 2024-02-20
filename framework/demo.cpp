#include "include/fate.h"

class CDemo : public CFateApp
{
public:
	CDemo() : m_btnQuit("Quit"), m_btnAction("Action"),
		m_itemList(7, 100, NULL, NULL)
	{
		SetWidth(800);
		SetHeight(800);
	}

	virtual void Draw()
	{
		m_bmp.Blit();
	}

	virtual bool InitFateApp()
	{
		m_btnQuit.SetId(1234);
		Add(m_btnQuit);

		m_btnAction.SetId(2345);
		m_btnAction.SetY(m_btnQuit.GetBottom() + 20);
		Add(m_btnAction);

		m_bmp.SetDestBitmap(m_pSystem->GetDoubleBuffer());
		m_bmp.SetX(100);
		m_bmp.SetY(100);
		m_bmp.Load("test.bmp");

		m_itemList.SetX(20);
		m_itemList.SetY(m_btnAction.GetBottom() + 20);
		m_itemList.AddItem("one");
		m_itemList.AddItem("two");
		m_itemList.AddItem("three");
		Add(m_itemList);

		return true;
	}

	virtual bool ButtonPressed(unsigned long id)
	{
		if (id == 1234)
		{
			return Exit();
		}
		return false;
	}

private:
	CFButton m_btnQuit;
	CFButton m_btnAction;
	CFBitmap m_bmp;
	CFItemList m_itemList;
};

extern CFateApp* CreateFateApp()
{
	return new CDemo();
}
