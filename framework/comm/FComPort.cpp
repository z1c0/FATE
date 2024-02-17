#include "FComPort.h"

#if defined(_WIN32)
  #include "../os/WIN32/FComPortImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FComPortImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FComPortImpl.h"
#else
  #error "unknown OS"
#endif