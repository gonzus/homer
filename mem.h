#ifndef MEM_H_
#define MEM_H_

#include <stdlib.h>
#include <string.h>

#define _MEM_ALLOC(scalar, type, size) \
  do { \
    scalar = (type) malloc(size); \
  } while (0)
#define _MEM_REALLOC(scalar, type, osize, nsize) \
  do { \
    scalar = (type) realloc(scalar, nsize); \
  } while (0)
#define _MEM_FREE(scalar, type, size) \
  do { \
    free(scalar); \
    scalar = 0; \
  } while (0)

void mem_init(void);
void mem_fini(void);

#if defined(MEM_CHECK) && MEM_CHECK >= 1

#define MEM_ALLOC(scalar, type, size) \
  do { \
    _MEM_ALLOC(scalar, type, size); \
    mem_alloc_called(__FILE__, __LINE__, scalar, 1, size); \
  } while (0)
#define MEM_REALLOC(scalar, type, osize, nsize) \
  do { \
    mem_free_called(__FILE__, __LINE__, scalar, 1, osize); \
    _MEM_REALLOC(scalar, type, osize, nsize); \
    mem_alloc_called(__FILE__, __LINE__, scalar, 1, nsize); \
  } while (0)
#define MEM_FREE(scalar, type, size) \
  do { \
    mem_free_called(__FILE__, __LINE__, scalar, 1, size); \
    _MEM_FREE(scalar, type, size); \
  } while (0)
#define MEM_STRDUP(tgt, src, len) \
  do { \
    mem_strdup(__FILE__, __LINE__, &tgt, src, len);   \
  } while (0)
#define MEM_STRDEL(str, len) \
  do { \
    mem_strdel(__FILE__, __LINE__, &str, len);   \
  } while (0)


extern long mem_alloc;
extern long mem_free;

int mem_alloc_called(const char* file,
                     int line,
                     void* var,
                     int count,
                     long size);
int mem_free_called(const char* file,
                    int line,
                    void* var,
                    int count,
                    long size);

int mem_strdup(const char* file,
               int line,
               char** tgt,
               const char* src,
               int len);
int mem_strdel(const char* file,
               int line,
               char** str,
               int len);

#else

#define MEM_ALLOC(scalar, type, size)           _MEM_NEW(scalar, type, size)
#define MEM_REALLOC(scalar, type, osize, nsize) _MEM_REALLOC(scalar, type, osize, nsize)
#define MEM_FREE(scalar, type, size)            _MEM_DEL(scalar, type, size)

#define MEM_STRDUP(tgt, src, len) \
  do { \
    tgt = 0; \
    if (!src) { \
      break; \
    } \
    int l = len <= 0 ? strlen(src) + 1 : len; \
    _MEM_ALLOC(tgt, char*, l); \
    memcpy(tgt, src, l); \
  } while (0)
#define MEM_STRDEL(str, len) \
  do { \
    _MEM_DEL(str, char*, len); \
  } while (0)

#endif /* #if !defined(MEM_CHECK) || MEM_CHECK < 1 */

#endif
