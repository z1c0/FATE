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

typedef unsigned long COLORREF;

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

#define _max(a, b) (((a) > (b)) ? (a) : (b)) 

#ifdef _LINUX

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <stdarg.h>
#include <unistd.h>
#include <libgen.h>

#define _stprintf sprintf
#define _tcscmp strcmp
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcslen strlen

#define _tfopen fopen

#define RGB(r, g, b) 0

#define SOCKET_ERROR -1

#else

#define _PALM_OS

  #include <PalmOS.h>
  #include <unix_stdarg.h>

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

// Macro for safe memory release.
#define SAFE_DELETE(p)  { if (p) { delete(p); (p)= NULL; } }

