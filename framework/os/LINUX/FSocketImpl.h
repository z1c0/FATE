#pragma once

class CFSocketImpl
{
public:
	CFSocketImpl();

  static bool InitSocketLibrary() { return true; }
  static bool CleanupSocketLibrary() { return true; }
};