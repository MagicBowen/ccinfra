#ifndef H05B2224D_B926_4FC0_A936_97B52B8A99DB
#define H05B2224D_B926_4FC0_A936_97B52B8A99DB

#include <ccinfra/core/Config.h>

#if __SUPPORT_NULL_PTR
#define __null_ptr__ nullptr
#else
#define __null_ptr__ 0
#endif

#define __notnull__(ptr) ptr != __null_ptr__
#define __null__(ptr) ptr == __null_ptr__

#endif

