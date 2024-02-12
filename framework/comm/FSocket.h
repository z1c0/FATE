#if defined(_WIN32) || defined(_WIN32_WCE)
  #include "FSocket_WIN32.h"
#else 
  #include "FSocket_PalmOS.h"
#endif