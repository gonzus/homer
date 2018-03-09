#include <stdio.h>
#include "mem.h"

int mem_unused = 0;

#if defined(MEM_CHECK) && MEM_CHECK >= 1

long mem_alloc = 0;
long mem_free = 0;

static int mem_inited = 0;

void mem_init()
{
    if (mem_inited) {
        return;
    }
    mem_inited = 1;

    atexit(mem_fini);
    mem_alloc = mem_free = 0;

#if defined(MEM_CHECK) && MEM_CHECK >= 1
    fprintf(stderr, "=== MEM BEG %ld %ld ===\n", mem_alloc, mem_free);
#endif
}

void mem_fini(void)
{
    if (!mem_inited) {
        return;
    }
    mem_inited = 0;

#if defined(MEM_CHECK) && MEM_CHECK >= 1
    fprintf(stderr, "=== MEM END %ld %ld ===\n", mem_alloc, mem_free);
    if (mem_alloc == mem_free) {
        fprintf(stderr, "=== MEM OK ===\n");
    } else {
        fprintf(stderr, "=== MEM ERR %ld BYTES ===\n", mem_alloc - mem_free);
    }
#endif
}

int mem_alloc_called(const char* file,
                     int line,
                     void* var,
                     int count,
                     long size)
{
    if (!var) {
        return 0;
    }
    if (size <= 0 || count <= 0) {
        return 0;
    }
    if (!mem_inited) {
        mem_init();
    }

    long total = size * count;

#if defined(MEM_CHECK) && MEM_CHECK >= 2
    fprintf(stderr, "=== MEM ALLOC %s %d %p %d %ld %ld ===\n",
            file, line, var, count, size, total);
#else
    UNUSED_PARAMETER(file);
    UNUSED_PARAMETER(line);
#endif

    mem_alloc += total;
    return total;
}

int mem_free_called(const char* file,
                    int line,
                    void* var,
                    int count,
                    long size)
{
    if (!var) {
        return 0;
    }
    if (size <= 0 || count <= 0) {
        return 0;
    }
    if (!mem_inited) {
        mem_init();
    }

    long total = size * count;

#if defined(MEM_CHECK) && MEM_CHECK >= 2
    fprintf(stderr, "=== MEM FREE %s %d %p %d %ld %ld ===\n",
            file, line, var, count, size, total);
#else
    UNUSED_PARAMETER(file);
    UNUSED_PARAMETER(line);
#endif

    mem_free += total;
    return total;
}

int mem_strdup(const char* file,
               int line,
               char** tgt,
               const char* src,
               int len)
{
    if (!tgt) {
        return 0;
    }
    *tgt = 0;

    if (!src) {
        return 0;
    }

    if (len <= 0) {
        len = strlen(src);
    }
    _MEM_ALLOC(*tgt, char*, len + 1);
    memcpy(*tgt, src, len);
    (*tgt)[len] = '\0';
    ++len;
#if defined(MEM_CHECK) && MEM_CHECK >= 2
    fprintf(stderr, "=== MEM STRNEW %p %d\n", *tgt, len);
#endif
    mem_alloc_called(file, line, *tgt, len, 1);
    return len;
}

int mem_strdel(const char* file,
               int line,
               char** str,
               int len)
{
    if (!str || !*str) {
        return 0;
    }

    if (len <= 0) {
        len = strlen(*str);
    }
    ++len;
#if defined(MEM_CHECK) && MEM_CHECK >= 2
    fprintf(stderr, "=== MEM STRDEL %p %d\n", *str, len);
#endif
    mem_free_called(file, line, *str, len, 1);
    _MEM_FREE(*str, char*, len);
    *str = 0;
    return len;
}

#else

void mem_init()
{
}

void mem_fini(void)
{
}

#endif /* #if defined(MEM_CHECK) && MEM_CHECK >= 1 */
