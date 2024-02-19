#pragma once

#define FATE_VERSION_MAJOR 1
#define FATE_VERSION_MINOR 2
#define FATE_VERSION_REVISION 0

class CFateVersion
{
  friend class CFateApp;

  CFateVersion()
  {
    _stprintf(m_short, TEXT("%d.%d.%d"), GetMajor(), GetMinor(), GetRevision());
    _stprintf(m_long, TEXT("FATE v%d.%d.%d - 2024"), GetMajor(), GetMinor(), GetRevision());
  }

public:
  int GetMajor() const { return FATE_VERSION_MAJOR; }
  int GetMinor() const { return FATE_VERSION_MINOR; }
  int GetRevision() const { return FATE_VERSION_REVISION; }
  const TCHAR* GetShortString() const { return m_short; }
  const TCHAR* GetLongString() const { return m_long; }

private:
  TCHAR m_short[10];
  TCHAR m_long[50];
};