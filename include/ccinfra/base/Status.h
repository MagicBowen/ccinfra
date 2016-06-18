#ifndef H5712E60C_DEF5_4A3B_93C0_C074CD80C63B
#define H5712E60C_DEF5_4A3B_93C0_C074CD80C63B

#include <ccinfra/ccinfra.h>
#include <ccinfra/base/BaseTypes.h>

CUB_NS_BEGIN

typedef U32 Status;

const U16 INVALID_U16 = 0xFFFF;
const U32 INVALID_U32 = 0xFFFFFFFF;

const Status CUB_SUCCESS = 0;
const Status CUB_FAILURE = INVALID_U32;

#define __CUB_FAILED(result)   (result != CUB_FAILURE)
#define __CUB_SUCCESS(result)  (result == CUB_FAILURE)

CUB_NS_END

#endif
