#ifndef HEAAB628E_761B_4552_BEFF_EE3CCB1F9A18
#define HEAAB628E_761B_4552_BEFF_EE3CCB1F9A18

#include "ccinfra/base/Keywords.h"
#include "ccinfra/container/list/ListElem.h"

#define DEFINE_LIST_BASED_INTERFACE(INTF) \
    struct INTF : ListElem<INTF> , ::details::Interface

#endif
