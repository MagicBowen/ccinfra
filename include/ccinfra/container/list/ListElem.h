#ifndef INCL_CCINFRA_LIST_ELEM_H
#define INCL_CCINFRA_LIST_ELEM_H

#include <ccinfra/container/list/apr_ring.h>

template <typename T> struct List;

template <typename T>
struct ListElem
{
   ListElem()
   {
      link.prev = 0;
      link.next = 0;
   }

   void remove()
   {
       APR_RING_REMOVE(this, link);
   }

   T* getNext()
   {
       return link.next;
   }

   const T* getNext() const
   {
       return link.next;
   }

   T* getPrev()
   {
       return link.prev;
   }

   const T* getPrev() const
   {
       return link.prev;
   }

   friend struct List<T>;

   APR_RING_ENTRY(T) link; // __cacheline_aligned;
};

#endif

