#include "FBitmap_WIN32.h"

//--------------------------------------------------------------------------------
CFBitmap::CFBitmap()
{
  m_hDestDC = NULL;
  Init();
}

//--------------------------------------------------------------------------------
CFBitmap::CFBitmap(const CFBitmap& destbmp)
{
  m_hDestDC = destbmp.m_hSourceDC;
  Init();
}

//--------------------------------------------------------------------------------
CFBitmap::~CFBitmap()
{
  CleanUp();
}

//--------------------------------------------------------------------------------
void CFBitmap::Init()
{
  m_hSourceDC    = NULL;
  m_hSaveDC      = NULL;
  m_hOldBmp      = NULL;
  m_hOldSaveBmp  = NULL;
  m_ulPaletteSize= 0;
  m_ulDataSize   = 0;
  m_iBitCount    = 0;
  m_iPlanes      = 0;
  m_pBits        = NULL;
  m_iPosX        = 0;
  m_iPosY        = 0;
  m_iWidth       = 0;
  m_iHeight      = 0;

  m_colBackground= RGB(255, 255, 255);
  m_colForeground= RGB(0, 0, 0);
  m_hPen  = CreatePen(PS_SOLID, 1, m_colForeground);
  m_hBrush= CreateSolidBrush(m_colBackground);
  m_hPenOld  = NULL;
  m_hBrushOld= NULL;
}

//--------------------------------------------------------------------------------
bool CFBitmap::IsValid() const
{
  return (m_hSourceDC != NULL);
}

//--------------------------------------------------------------------------------
/// Releases all memory that is allocated by the bitmap
void CFBitmap::CleanUp()
{
  // clean up of main device context
  if (m_hSourceDC) {
    HBITMAP hCurrBmp;
    
    hCurrBmp= (HBITMAP)SelectObject(m_hSourceDC, m_hOldBmp);
    if (hCurrBmp) DeleteObject(hCurrBmp);
	  DeleteDC(m_hSourceDC);
    m_hSourceDC= NULL;
  }
  // clean up of save-device context
  if (m_hSaveDC) {
    HBITMAP hCurrBmp;
    
    hCurrBmp= (HBITMAP)SelectObject(m_hSaveDC, m_hOldSaveBmp);
    if (hCurrBmp) DeleteObject(hCurrBmp);
	  DeleteDC(m_hSaveDC);
    m_hSaveDC= NULL;
  }

  if (m_hPenOld) (HPEN)SelectObject(m_hSourceDC, m_hPenOld);
  if (m_hBrushOld) (HBRUSH)SelectObject(m_hSourceDC, m_hBrushOld);
  if (m_hPen) DeleteObject(m_hPen);
  if (m_hBrush) DeleteObject(m_hBrush);
}

//--------------------------------------------------------------------------------
/// Creates an new bitmap of specified dimensions.
/// Previously allocated memory gets released automatically
bool CFBitmap::Create(int iWidth, int iHeight)
{
  // cleanup necessary?
  if (m_hSourceDC) {
    CleanUp();
    Init();
  }

  m_hSourceDC= CreateCompatibleDC(m_hDestDC);
  if (!m_hSourceDC) return(FALSE);

  // fill bitmapinfo structure
  BITMAPINFO bmi= {0};
  bmi.bmiHeader.biBitCount     = 24;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biClrUsed      = 0;
  bmi.bmiHeader.biCompression  = 0;
  bmi.bmiHeader.biHeight       = iHeight;
  bmi.bmiHeader.biPlanes       = 1;
  bmi.bmiHeader.biSize         = 40;
  bmi.bmiHeader.biSizeImage    = iHeight * iWidth * 3;
  bmi.bmiHeader.biWidth        = iWidth;
  bmi.bmiHeader.biXPelsPerMeter= 3780;
  bmi.bmiHeader.biYPelsPerMeter= 3780;
  bmi.bmiColors[0].rgbBlue     = 0;
  bmi.bmiColors[0].rgbGreen    = 0;
  bmi.bmiColors[0].rgbRed      = 0;
  bmi.bmiColors[0].rgbReserved = 0;   

  HBITMAP hBmp= CreateDIBSection(m_hDestDC, &bmi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0);	
  m_hSourceDC= CreateCompatibleDC(m_hDestDC);
  m_hOldBmp= (HBITMAP)SelectObject(m_hSourceDC, hBmp);  
  
  m_iWidth = iWidth;
  m_iHeight= iHeight;
  
  return(TRUE);
}

//------------------------------------------------------------------------------
void CFBitmap::SetDestBitmap(const CFBitmap& bmp)
{
  m_hDestDC = bmp.m_hSourceDC;
}

//--------------------------------------------------------------------------------
/// Fills the whole bitmap with the color specified by "colFill".
bool CFBitmap::SolidFill(const COLORREF colFill)
{
  if (m_hSourceDC) {
    // set the rectangle which is to be filled
    RECT rectBmp;
    rectBmp.left  = 0;
    rectBmp.top   = 0;
    rectBmp.right = m_iWidth;
    rectBmp.bottom= m_iHeight;
    // create fill brush
    HBRUSH hFillBrush= CreateSolidBrush(colFill);
    FillRect(m_hSourceDC, &rectBmp, hFillBrush);
    DeleteObject(hFillBrush);
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Blits the bitmap to its preset destination device context.
bool CFBitmap::Blit()
{
  return (BitBlt(m_hDestDC, m_iPosX, m_iPosY, m_iWidth, m_iHeight, m_hSourceDC, 0, 0, SRCCOPY) != 0); 
}

//--------------------------------------------------------------------------------
/// Blits the bitmap to its preset destination device context.
bool CFBitmap::Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty)
{
  return (BitBlt(m_hDestDC, srcx, srcy, srcw, srch, m_hSourceDC, destx, desty, SRCCOPY) != 0); 
}


//--------------------------------------------------------------------------------
bool CFBitmap::ClipBlit(int iWidth, int iHeight) 
{
  if ((m_iWidth < iWidth)||(m_iHeight < iHeight)) {
    if (StretchBlt(m_hDestDC, m_iPosX, m_iPosY, iWidth, iHeight, m_hSourceDC, 0, 0, m_iWidth, m_iHeight, SRCCOPY)) {
      m_iWidth = iWidth;
      m_iHeight= iHeight;
      return(TRUE);
    }
  } else {
    if (StretchBlt(m_hSourceDC, m_iPosX, m_iPosY, iWidth, iHeight, m_hDestDC, 0, 0, m_iWidth, m_iHeight, SRCCOPY)) {
      m_iWidth = iWidth;
      m_iHeight= iHeight;
      return(TRUE);
    }
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Blits the bitmap to its preset destination device context, but at the
/// maximum dimensions specified by "iWidth" and "iHeight".
bool CFBitmap::Blit(int iWidth, int iHeight) 
{
  return(BitBlt(m_hDestDC, m_iPosX, m_iPosY, 
                m_iWidth  > iWidth  ? iWidth  : m_iWidth, 
                m_iHeight > iHeight ? iHeight : m_iHeight,
                m_hSourceDC, 0, 0, SRCCOPY) != 0);
}

//--------------------------------------------------------------------------------
bool CFBitmap::StretchBlit(int iWidth, int iHeight)
{
  return (StretchBlt(m_hDestDC, m_iPosX, m_iPosY, iWidth, iHeight, 
          m_hSourceDC, 0, 0, m_iWidth, m_iHeight, SRCCOPY) != 0);
}

//--------------------------------------------------------------------------------
bool CFBitmap::TransBlit(COLORREF colTrans)
{
#ifdef _WIN32_WCE
  return (TransparentImage(m_hDestDC, m_iPosX, m_iPosY, m_iWidth, m_iHeight, 
          m_hSourceDC, 0, 0, m_iWidth, m_iHeight, colTrans) != 0);
#else
  return (::TransparentBlt(m_hDestDC, m_iPosX, m_iPosY, m_iWidth, m_iHeight, 
            m_hSourceDC, 0, 0, m_iWidth, m_iHeight, colTrans) != 0);
#endif
}

//--------------------------------------------------------------------------------
/// Loads a bitmap from the specified file.
bool CFBitmap::Load(LPTSTR szFileName)
{
  HANDLE hFile;
  DWORD dwSize;
  DWORD dwBytesRead= 0;
  bool bOK;
  char *pszData;
  
  hFile= CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, NULL);
  // CreateFile successful?
  if (hFile == INVALID_HANDLE_VALUE) return(FALSE);
  
  dwSize= GetFileSize(hFile, NULL);
  if (dwSize == 0xFFFFFFFF) return(FALSE);

  // allocate memory for reading the file
  pszData= (char*)malloc(dwSize);
  if (!pszData) {
    MessageBox(NULL, TEXT("Memory allocation failed"), TEXT("ERROR"), MB_OK);
    return(FALSE);
  }
  
  // read the bitmap file
  bOK = (::ReadFile(hFile, (LPVOID)pszData, dwSize, &dwBytesRead, NULL) != 0);
  if ((!bOK)||(dwBytesRead != dwSize)) {
    CloseHandle(hFile);
    free(pszData);
    return(FALSE);
  }
 
  CloseHandle(hFile);
  bOK= Load(pszData, dwSize);
  free(pszData);
  return(bOK);
}

//--------------------------------------------------------------------------------
/// Loads a bitmap from the specfied resource-identifier.
bool CFBitmap::Load(int wResourceID)
{
  HRSRC hResource;
  DWORD dwSize;
  HGLOBAL hGlobal;
  HMODULE hMod;
  char* pTempResource;
  char* pResource;
  const int iHeaderSize= 14;  // size of BMP-File-Header
  bool bRet= FALSE;

  // get handle to current module
  hMod= GetModuleHandle(NULL);
  if (hMod == NULL) return(FALSE);
  
  // locate the resource
  hResource= FindResource(hMod, MAKEINTRESOURCE(wResourceID), RT_BITMAP); 
  if (hResource == NULL) return(FALSE);
  
  dwSize= SizeofResource(hMod,  hResource);
  // reconstruct file header
  pTempResource = (char*)malloc(dwSize  + iHeaderSize);
  memset(pTempResource, 0, iHeaderSize);
  pTempResource[0] = 0x42;
  pTempResource[1] = 0x4D;
  pTempResource[10]= 0x36;
  pTempResource[11]= 0x04;
  
  // get a pointer to resource bytes
  hGlobal= LoadResource(hMod, hResource); 
  if (hGlobal) {
    pResource= (char*)LockResource(hGlobal);
    memcpy(pTempResource + iHeaderSize, pResource, dwSize);
    bRet= Load(pTempResource, dwSize + iHeaderSize);
  }  
  
  free(pTempResource);  
  return(bRet);
}

//--------------------------------------------------------------------------------
/// Loads a bitmap from a memory file.
bool CFBitmap::Load(char *pszData, DWORD dwSize)
{
  BITMAPFILEHEADER bmpfh;
  HBITMAP hBmp;
  char *pTemp= NULL;
  char *pNewBits= NULL;
  DWORD dwBuffPos= 0;

  // cleanup necessary?
  if (m_hSourceDC) {
    CleanUp();
    Init();
  }

  // read bitmap-file-header
  if (sizeof(BITMAPFILEHEADER) > dwSize) return(FALSE);  
  memcpy((LPVOID)&bmpfh, pszData, sizeof(BITMAPFILEHEADER));
  dwBuffPos+= sizeof(BITMAPFILEHEADER);

  // check if it is a valid bitmap-file
  if ((bmpfh.bfType != 0x4D42)||(bmpfh.bfReserved1 != 0)||(bmpfh.bfReserved2 != 0)) {
    return(FALSE);
  }

	// allocate memory for bitmap info
	m_pBits= (char*)malloc(sizeof(BITMAPINFO));
  if(!m_pBits) {
		return(FALSE);
	}

	// read bitmapinfoheader
  if (dwBuffPos + sizeof(BITMAPINFOHEADER) > dwSize) {
   	free(m_pBits);
    return(FALSE);  
  }
  memcpy(m_pBits, pszData + dwBuffPos, sizeof(BITMAPINFOHEADER));
  dwBuffPos+= sizeof(BITMAPINFOHEADER);
	
	if (((LPBITMAPINFOHEADER)m_pBits)->biSize != sizeof(BITMAPINFOHEADER)) {
		free(m_pBits);
		return(FALSE);
	}

  // store read data
  m_iWidth   = ((LPBITMAPINFOHEADER)m_pBits)->biWidth;
  m_iHeight  = ((LPBITMAPINFOHEADER)m_pBits)->biHeight;
  m_iBitCount= ((LPBITMAPINFOHEADER)m_pBits)->biBitCount;
  m_iPlanes  = ((LPBITMAPINFOHEADER)m_pBits)->biPlanes;
  
  // size of palette is needed
	if (((LPBITMAPINFOHEADER)m_pBits)->biClrUsed) 
    m_ulPaletteSize= ((LPBITMAPINFOHEADER)m_pBits)->biClrUsed * sizeof(RGBQUAD);
  else if (m_iBitCount <= 8)
    m_ulPaletteSize= (1 << m_iBitCount) * sizeof(RGBQUAD);
  else {
    m_ulPaletteSize= 0;
    bmpfh.bfOffBits= sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
  }

  // size of bitmap data is needed
  m_ulDataSize= (((m_iWidth * m_iPlanes * m_iBitCount + 31) / 32) * 4) * m_iHeight;

	// allocate memory for whole bitmap
	pTemp= (char*)realloc(m_pBits, sizeof(BITMAPINFOHEADER) + m_ulPaletteSize + m_ulDataSize);
  if (!pTemp) {
		free(m_pBits);
		return(FALSE);
	}
	m_pBits= pTemp;


	// if there is a palette read it
  if (m_ulPaletteSize) {
  	// read bitmapinfoheader
    if (dwBuffPos + m_ulPaletteSize > dwSize) {
			free(m_pBits);
      return(FALSE);  
    }
    memcpy(((LPBITMAPINFO)m_pBits)->bmiColors, pszData + dwBuffPos, m_ulPaletteSize);
    dwBuffPos+= m_ulPaletteSize;
	}

	if (bmpfh.bfOffBits) {
    // move to actual bitmap data
    if (bmpfh.bfOffBits > dwSize) {
			free(m_pBits);
      return(FALSE);  
    }
    dwBuffPos= bmpfh.bfOffBits;
	}

  // read the actual bitmap data
  if (dwBuffPos + m_ulDataSize > dwSize) {
		free(m_pBits);
    return(FALSE);  
  }
  memcpy(m_pBits + *(LPDWORD)m_pBits + m_ulPaletteSize, pszData + dwBuffPos, m_ulDataSize);
  dwBuffPos+= m_ulDataSize;  

  // all data is read from file
  hBmp= CreateDIBSection(m_hDestDC, (BITMAPINFO*)m_pBits, DIB_RGB_COLORS, (void**)&pNewBits, NULL, 0);
  memcpy(pNewBits, m_pBits + *(LPDWORD)m_pBits + m_ulPaletteSize, m_ulDataSize);
	
  m_hSourceDC= CreateCompatibleDC(m_hDestDC);
  m_hOldBmp= (HBITMAP)SelectObject(m_hSourceDC, hBmp);
  
  return(TRUE);   
}

//--------------------------------------------------------------------------------
bool CFBitmap::CreateSaveBitmap(const CFBitmap& bmp)
{
  HDC hdc = bmp.m_hSourceDC;

  if (hdc) {
    HBITMAP hTempBmp = CreateCompatibleBitmap(hdc, m_iWidth, m_iHeight);
    if (!hTempBmp) {
      return false;
    }
    m_hSaveDC = CreateCompatibleDC(hdc);
    if (!m_hSaveDC) {
      return false;
    }
    m_hOldSaveBmp = (HBITMAP)SelectObject(m_hSaveDC, hTempBmp);
    return true;
  }
  
  return false;
}

//--------------------------------------------------------------------------------
bool CFBitmap::SaveUnder(const CFBitmap& bmp)
{
  // Check if DC for saving was already created; if not, do so.
  if (!m_hSaveDC) {
    CreateSaveBitmap(bmp);
  }

  HDC hdc = bmp.m_hDestDC;
  if ((m_hSaveDC)&&(hdc)) {
    return (BitBlt(m_hSaveDC, 0, 0, m_iWidth, m_iHeight, hdc, m_iPosX, m_iPosY, SRCCOPY) != 0);
  }
  return false;
}

//--------------------------------------------------------------------------------
bool CFBitmap::RestoreUnder(const CFBitmap& bmp)
{
  HDC hdc = bmp.m_hDestDC;
  if ((m_hSaveDC)&&(hdc)) {
    return (BitBlt(hdc, m_iPosX, m_iPosY, m_iWidth, m_iHeight, m_hSaveDC, 0, 0, SRCCOPY) != 0);
  }
  return false;
}

//--------------------------------------------------------------------------------
CFBitmap& CFBitmap::operator=(const CFBitmap &bmp)
{
  // Avoid self-assinging.
  if (this == &bmp) {
    return *this;
  }

  m_hDestDC = bmp.m_hDestDC;
  Create(bmp.m_iWidth, bmp.m_iHeight);
  BitBlt(m_hSourceDC, 0,0, m_iWidth, m_iWidth, bmp.m_hSourceDC, 0, 0, SRCCOPY);

  return *this;
}

//--------------------------------------------------------------------------------
CFBitmap* CFBitmap::operator=(CFBitmap *bmp)
{
  *this= *bmp;
  return(this);
}

//------------------------------------------------------------------------------
bool CFBitmap::SetBits(unsigned char *pBits, int iSize)
{
  if ((m_pBits)&&(m_iWidth * m_iHeight * 3 >= iSize)) {
    memcpy(m_pBits, pBits, iSize);
	return true;
  }

  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::SaveToFile(const TCHAR *pszFileName)
{
  BITMAPINFOHEADER bmpih;
  BITMAPFILEHEADER bmpfh;
  HANDLE hFile;
  DWORD dwWritten;

  bmpfh.bfType     = 0x4D42;
  bmpfh.bfReserved1= 0;
  bmpfh.bfReserved2= 0;
  bmpfh.bfOffBits  = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
  bmpfh.bfSize     = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_iHeight * m_iWidth * 3;

  hFile= CreateFile(pszFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    
  WriteFile(hFile, &bmpfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

  memset(&bmpih, 0, sizeof(BITMAPINFOHEADER));
  bmpih.biSize       = sizeof(BITMAPINFOHEADER);
  bmpih.biPlanes     = 1;
  bmpih.biWidth      = m_iWidth;
  bmpih.biHeight     = m_iHeight;
  bmpih.biBitCount   = 24;
  bmpih.biCompression= BI_RGB;
  WriteFile(hFile, &bmpih, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);

  WriteFile(hFile, m_pBits, m_iHeight * m_iWidth * 3, &dwWritten, NULL);
  CloseHandle(hFile);

  return(true);
};


//------------------------------------------------------------------------------
COLORREF CFBitmap::SetColor(COLORREF col)
{
  COLORREF colRet= m_colForeground;

  if (m_colForeground != col) {
    m_colForeground = col;

    // create new pen
    DeleteObject(m_hPen);
    m_hPen = CreatePen(PS_SOLID, 1, m_colForeground);

    if (m_hSourceDC) {
      if (m_hBrushOld) SelectObject(m_hSourceDC, m_hPenOld);
      m_hPenOld = (HPEN)SelectObject(m_hSourceDC, m_hPen);
    }
  }

  return colRet;
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetTextColor(COLORREF col)
{
  COLORREF colOld = ::GetTextColor(m_hSourceDC);
 ::SetTextColor(m_hSourceDC, col);
  return colOld;
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetBackgroundColor(COLORREF col)
{
  COLORREF colRet = m_colBackground;

  if (m_colBackground != col) {
    m_colBackground = col;

    // create new brush
    DeleteObject(m_hBrush);
    m_hBrush = CreateSolidBrush(m_colBackground);

    if (m_hSourceDC) {
      if (m_hBrushOld) SelectObject(m_hSourceDC, m_hBrushOld);
      m_hBrushOld = (HBRUSH)SelectObject(m_hSourceDC, m_hBrush);
      SetBkColor(m_hSourceDC, m_colBackground);
   }
  }
  
  return colRet;
}

//------------------------------------------------------------------------------
bool CFBitmap::CalcRectForText(const TCHAR *pszText, RECT& rect)
{
  HWND hWnd = ::GetDesktopWindow();
  HDC hdc = ::GetDC(hWnd);
  int ret = ::DrawText(hdc, pszText, _tcslen(pszText), &rect, DT_CALCRECT);
  ::ReleaseDC(hWnd, hdc);
  return (ret != 0);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawText(const TCHAR *pszText, RECT& rect)
{
  return (::DrawText(m_hSourceDC, pszText, _tcslen(pszText), &rect, DT_SINGLELINE|DT_LEFT|DT_VCENTER) != 0);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(const RECT& rect)
{
  return DrawFilledRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight)
{
  return (::Rectangle(m_hSourceDC, iLeft, iTop, iLeft + iWidth, iTop + iHeight) != 0);
 }

//------------------------------------------------------------------------------
bool CFBitmap::DrawPolygon(POINT *points, int count)
{
  return (::Polygon(m_hSourceDC, points, count) != 0);
}