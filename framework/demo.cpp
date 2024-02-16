#include "include/fate.h"

class CDemo : public CFateApp
{
};

extern CFateApp* CreateFateApp()
{
	return new CDemo();
}
