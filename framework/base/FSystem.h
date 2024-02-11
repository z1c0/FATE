#pragma once

#if defined(_WIN32_WCE)
  #include "WIN32_WCE/FSystem.h"
#elif defined (_WIN32)
  #include "WIN32/FSystem.h"
#else
  #error "PalmOS"
  #include "PalmOS/FSystem.h"
#endif