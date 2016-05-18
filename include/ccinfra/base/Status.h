#ifndef H5712E60C_DEF5_4A3B_93C0_C074CD80C63B
#define H5712E60C_DEF5_4A3B_93C0_C074CD80C63B

#include <ccinfra/base/BaseTypes.h>

typedef U32 Status;

const U16 INVALID_U16 = 0xFFFF;
const U16 INVALID_U32 = 0xFFFFFFFF;

const Status SUCCESS = 0;
const Status FAILURE = INVALID_U32;

#define __FAILED(result)  (result != SUCCESS)

#endif
