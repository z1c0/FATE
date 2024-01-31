#ifndef __VE_PROTOCOL_H___
#define __VE_PROTOCOL_H___


//--------------------------------------------------------------------------------
/// Constants
#define MAX_OBJ_NAME_LEN   64
#define MAX_ATTR_NAME_LEN  64

//--------------------------------------------------------------------------------
/// General packet format.
typedef struct __tagVE_PACKET
{
  unsigned int uiKind;
  unsigned int uiExtra;
  unsigned int uiLen;
} VE_PACKET, *LPVP_PACKET;

//--------------------------------------------------------------------------------
/// Packet format for transferring a set (3 floats) of 3D data.
typedef struct __tagVE_PACKET_3D
{
  unsigned long ulID;
  float fVal1;
  float fVal2;
  float fVal3;
} VE_PACKET_3D, *LPVP_PACKET_3D;

//--------------------------------------------------------------------------------
/// Packet format for transferring basic data of a CFVEObj
typedef struct __tagVE_PACKET_OBJ
{
  unsigned long ulID;
  char szName[MAX_OBJ_NAME_LEN];
  float fPosX;
  float fPosY;
  float fPosZ;
  float fAzim;
  float fElev;
  float fRoll;
  float fScaleX;
  float fScaleY;
  float fScaleZ;
  int iExtra;  // number of extra attributes
} VE_PACKET_OBJ, *LPVP_PACKET_OBJ;

//--------------------------------------------------------------------------------
/// Packet format for transferring extra attributes
typedef struct __tagVE_PACKET_ATTR
{
  char szName[MAX_ATTR_NAME_LEN];
  unsigned int uiLen;
} VE_PACKET_ATTR, *LPVP_PACKET_ATTR;


//--------------------------------------------------------------------------------
/// Protocol commands
#define REQ_LIST                                   0x0001
#define REQ_LIST_OK                                0x0002
#define REQ_LIST_ERR                               0x0003

#define REQ_OBJ                                    0x0004
#define REQ_OBJ_OK                                 0x0005
#define REQ_OBJ_ERR                                0x0006

#define REQ_CHANGE                                 0x0007
#define REQ_CHANGE_OK                              0x0008
#define REQ_CHANGE_ERR                             0x0009
#define CHANGE_POS                                 0xA001
#define CHANGE_ORIENTATION                         0xA002
#define CHANGE_SCALE                               0xA003
#define CHANGE_EXTRA                               0xA004
#define CHANGE_ALL                                 0xA005


#endif //  __VE_PROTOCOL_H___