#pragma once

#ifdef _WIN32

#ifdef _WIN32_WCE
  #include "WIN32_WCE/FSystem.h"
#else
  #include "WIN32/FSystem.h"
#endif

#else
  #include "PalmOS/FSystem.h"
#endif