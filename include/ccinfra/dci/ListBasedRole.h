#ifndef H05B2224D_B926_4FC0_A936_67B52B8A98DE
#define H05B2224D_B926_4FC0_A936_67B52B8A98DE

#include <ccinfra/dci/Role.h>
#include <ccinfra/ctnr/list/ListElem.h>

#define DEFINE_LIST_BASED_ROLE(role) \
   DEFINE_ROLE(role) EXTENDS(ListElem<role>)

#endif
