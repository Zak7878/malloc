#define _GNU_SOURCE

#include <sys/mman.h>
#include <malloc.h>
#include <unistd.h>
#include "stdint.h"
#include "malloc.h"
static struct page *page = NULL;
void *init(size_t size)
{
  if(page == NULL)
  {
    size_t p = sysconf(_SC_PAGESIZE); //size de la page
    size_t nbpage =(sizeof(struct metadata) + size + sizeof(struct page)) / p+1; //nombre de page
    nbpage = nbpage * p; //taille total de la page
    page = mmap(NULL,nbpage, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
  
    page->next = NULL;
    page->meta = (void*)((uintptr_t)page + sizeof(struct page));
    page->size = nbpage - sizeof(struct page) - sizeof(struct metadata) - size;
    page->meta->size = size;
    page->meta->state = 1;
    page->meta->next = NULL;
    page->meta->npage++;
    return (void*)((uintptr_t)page + sizeof(struct metadata) + sizeof(struct
    page));
  }
  return NULL;
}

void *mymalloc(size_t size)
{
  if(page==NULL)
    return init(size);
  struct page *tmp = page;
  while(tmp != NULL)
  {
    struct metadata *m = page->meta;
    while(m->next != NULL && !(m->state == 0 && m->size > size))
    {
      m = m->next;
    }
    if(m->state == 0 && m->size > size)
      return (void*)((uintptr_t)m + sizeof(struct metadata));
    if(m->next ==NULL && tmp->size > sizeof(struct metadata) + size)
    {
      tmp->size -= sizeof(struct metadata) + size;
      m->next = (void*)((uintptr_t)m + sizeof(struct metadata) + m->size);
      m->next->state = 1;
      m->next->size = size;
      return (void*)((uintptr_t)m->next + sizeof(struct metadata));
    }
    if(m->next == NULL && tmp->size < sizeof(struct metadata) + size)
    {
      size_t p = sysconf(_SC_PAGESIZE);
      size_t nbpage = (sizeof(struct page) + size + sizeof(struct metadata))/p+1;
      nbpage = nbpage * p;
      page = mmap(NULL,nbpage, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
      page->next = NULL;
      page->meta = (void*)((uintptr_t)page + sizeof(struct page));
      page->size = p - sizeof(struct page) - sizeof(struct metadata) - size;
      page->meta->size = size;
      page->meta->state = 1;
      page->meta->next = NULL;
      page->meta->npage++;
      return (void*)((uintptr_t)page + sizeof(struct metadata) + sizeof(struct
      page));
      
    }
      return init(size);
    
     tmp = tmp->next;

    }
  return NULL;
}




void myfree(void *ptr)
{
  int *p = (int*)ptr;
  int len;
  p--;
  len = *p;
  munmap((void*)p,len);

}

void *calloc(size_t number, size_t size)
{
  size_t alloc_s = number* size;
  void *tmp = mymalloc(alloc_s);
  if(tmp){
    memset(tmp,0,alloc_s);
    return tmp;
  }
  return NULL;

}


int main()
{
  mymalloc(10000);
  mymalloc(4000);
  mymalloc(2048);
  mymalloc(1000); 
  calloc(20,100);
  void *s = mymalloc(500);
  myfree(s);
  mymalloc(5000);
  
  return 0;
}
