#ifndef __GUI_PROTOCOL_H___
#define __GUI_PROTOCOL_H___

//--------------------------------------------------------------------------------
/// General packet format.
typedef struct __tagGUI_PACKET
{
  unsigned int uiCmd;
  unsigned int uiKind;
  unsigned int uiID;
  int iPosX;
  int iPosY;
  unsigned int uiLen;
} GUI_PACKET, *LPGUI_PACKET;


/// Protocol commands
#define WIDGET_PALMIST_READY                         0x0001
#define WIDGET_ADD                                   0x0002
#define WIDGET_REMOVE                                0x0003

/// widget kinds
#define WIDGET_BUTTON                                0xF001
#define WIDGET_SWITCH                                0xF002
#define WIDGET_SLIDER                                0xF003
#define WIDGET_MENU                                  0xF004
#define WIDGET_ITEMLIST                              0xF005

#endif  // __GUI_PROTOCOL_H___

