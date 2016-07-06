#ifndef MALLOC_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

struct metadata
{
    size_t size;
    size_t state;
    size_t npage;
    struct metadata *next;    
};

struct page
{
   struct page *next;
   size_t size;
  struct metadata *meta;
};

#endif /* MALLOC_H */
