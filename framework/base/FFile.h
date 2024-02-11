#pragma once

#if defined(_WIN32) || defined(_WIN32_WCE)
  #include "WIN32/FFile.h"
#else
  #error "PalmOS"
#endif