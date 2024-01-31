#ifndef __IFDROPLISTLISTENER__H__
#define __IFDROPLISTLISTENER__H__

//------------------------------------------------------------------------------
class IFDropListListener 
{
public:
  virtual bool DropListExpanded(unsigned long ulListID)= 0;
  virtual bool DropListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry)= 0;
  virtual bool DropListCollapsed(unsigned long ulListID)= 0;
};

#endif  // __IFDROPLISTLISTENER__H__