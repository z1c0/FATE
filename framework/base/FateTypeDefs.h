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

#define _PALM_OS
 
  #include <PalmOS.h>
  #include <unix_stdarg.h>

//
// PalmOS specfic
//

#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

//------------------------------------------------------------------------------
/// String functions
#define TEXT
typedef char* LPTSTR;
typedef const char* LPCTSTR;
typedef char TCHAR;
typedef unsigned long DWORD;

#define _tcscmp StrCompare
#define _tcslen StrLen
#define _tcscpy StrCopy
#define sprintf StrPrintF
#define strlen StrLen
#define memcpy MemMove

//------------------------------------------------------------------------------
/// RGB

#define RGB(r, g, b) (unsigned short)((((unsigned char)(r)&0xF8)<<8)|(((unsigned char)(g)&0xFC)<<3)|(((unsigned char)(b)&0xF8)>>3))
typedef unsigned long COLORREF;


#define GetRValue(rgb) ((unsigned char)(((unsigned short)(rgb)&0xF800)>>8))
#define GetGValue(rgb) ((unsigned char)(((unsigned short)rgb)>>3)&0xFC)
#define GetBValue(rgb) ((((unsigned char)rgb) << 3) & 0xF8)


//------------------------------------------------------------------------------

#define MAX_PATH 256

struct RECT
{
  int left;
  int top;
  int right;
  int bottom;
};

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
  DM_LANDSCAPE_FLIPPED,
};

// Macro for safe memory release.
#define SAFE_DELETE(p)  { if (p) { delete(p); (p)= NULL; } }

