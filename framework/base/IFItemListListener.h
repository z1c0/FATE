#ifndef __IFITEMLISTLISTENER__H__
#define __IFITEMLISTLISTENER__H__

//--------------------------------------------------------------------------------
class IFItemListListener
{
public:
  virtual bool ItemListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry)= 0;
};

#endif // __IFITEMLISTLISTENER__H__
