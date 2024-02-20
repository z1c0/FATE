#pragma once

#ifdef _WIN32
  #include <windows.h>
  #include <winsock.h>
  #include <stdio.h>
  #include <TCHAR.h>

#ifdef _WIN32_WCE
  #include <aygshell.h>
  #include <gx.h>
#endif

#else

// not windows

#define TEXT
typedef char* LPTSTR;
typedef const char* LPCTSTR;
typedef char TCHAR;
typedef unsigned long DWORD;
typedef unsigned int UINT;

#define MAX_PATH 250

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

struct POINT
{
  int x;
  int y;
};

struct RECT
{
  int left;
  int top;
  int right;
  int bottom;
};

#ifdef _LINUX

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <stdarg.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>

#define _stprintf sprintf
#define _tcscmp strcmp
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcslen strlen
#define _tcscat strcat
#define _tcstok strtok
#define _tcsrchr strrchr

#define _tfopen fopen

#define RGB(r, g, b) COLORREF(r, g, b)

struct COLORREF
{
  COLORREF() : r(0), g(0), b(0) {}
  COLORREF(int r, int g, int b) : r(r), g(g), b(b) {}
  int r;
  int g;
  int b;
};

#else

#define _PALM_OS

  #include <PalmOS.h>
  #include <unix_stdarg.h>
  #include <assert.h>

//
// PalmOS specfic
//

//------------------------------------------------------------------------------
/// String functions
#define _tcscmp StrCompare
#define _tcslen StrLen
#define _tcscpy StrCopy
#define _stprintf StrPrintF
#define strlen StrLen
#define memcpy MemMove

//------------------------------------------------------------------------------
/// RGB

typedef unsigned short COLORREF;

#define RGB(r, g, b) (unsigned short)((((unsigned char)(r)&0xF8)<<8)|(((unsigned char)(g)&0xFC)<<3)|(((unsigned char)(b)&0xF8)>>3))

#define GetRValue(rgb) ((unsigned char)(((unsigned short)(rgb)&0xF800)>>8))
#define GetGValue(rgb) ((unsigned char)(((unsigned short)rgb)>>3)&0xFC)
#define GetBValue(rgb) ((((unsigned char)rgb) << 3) & 0xF8)

#endif 

#endif


//------------------------------------------------------------------------------

//
// Common
//

// Socket constants.
#define SOCKET_TIMEOUT      SOCKET_ERROR - 1
#define DEFAULT_TIMEOUT     5
#define NO_TIMEOUT          0xFFFF

class CFBitmap;

struct ITEMLISTENTRY
{
  TCHAR *pszItem;
  TCHAR *pszAddInfo;
  unsigned long ulIndex;
  unsigned long ulUser;
  CFBitmap *pBmp;
  struct ITEMLISTENTRY *pNext;
};

// Drawing modes determine how the screen contents are drawn (flipped / landscape).
enum EFateDrawMode
{
  DM_PORTRAIT,
  DM_PORTRAIT_FLIPPED,
  DM_LANDSCAPE,     
  DM_LANDSCAPE_FLIPPED
};

#define _max(a, b) (((a) > (b)) ? (a) : (b)) 

// Macro for safe memory release.
#define SAFE_DELETE(p)  { if (p) { delete(p); (p)= NULL; } }

#define COLOR_CORNFLOWER_BLUE RGB(100, 149, 237)

//--------------------------------------------------------------------------------
// Messages for exchanging information about events triggered by user interface controls.
#define FATE_EVENT_ID_BUTTONPRESS           0xBE01
#define FATE_EVENT_ID_BUTTONRELEASE         0xBE02
#define FATE_EVENT_ID_MENUSELECTION         0xBE03
#define FATE_EVENT_ID_ITEMLISTSELECT        0xBE04
#define FATE_EVENT_ID_DROPLISTEXPAND        0xBE05
#define FATE_EVENT_ID_DROPLISTSELECT        0xBE06
#define FATE_EVENT_ID_DROPLISTCOLLAPSE      0xBE07
#define FATE_EVENT_ID_SLIDERCHANGE          0xBE08
#define FATE_EVENT_ID_CLIENTCONNECT         0xBE09
#define FATE_EVENT_ID_REDRAW                0xBE10
#define FATE_EVENT_ID_TIMER                 0xBE11
