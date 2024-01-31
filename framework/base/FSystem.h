#ifdef _WIN32

#ifdef _WIN32_WCE
  #include "FSystem_WIN32_WCE.h"
#else
  #include "FSystem_WIN32.h"
#endif

#else
  #include "FSystem_PalmOS.h"
#endif