#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "log.h"
#include "block.h"

#define BLOCK_DEBUG 0
#define BLOCK_DEFAULT_SIZE 32

#if defined(BLOCK_DEBUG) && BLOCK_DEBUG > 0
static int block_print(Block* block);
#endif

Block* block_create(int size)
{
    Block* block;
    MEM_ALLOC(block, Block*, sizeof(Block));
    block->size = size <= 0 ? BLOCK_DEFAULT_SIZE : size;
    MEM_ALLOC(block->data, char*, block->size);
    block_reset(block);
    return block;
}

Block* block_clone(Block* block)
{
    if (!block) {
        return 0;
    }
    Block* clone;
    MEM_ALLOC(clone, Block*, sizeof(Block));
    clone->size = block->size;
    clone->next = block->next;
    MEM_ALLOC(clone->data, char*, clone->size);
    memcpy(clone->data, block->data, clone->next);
    return clone;
}

void block_destroy(Block* block)
{
    if (!block) {
        return;
    }
    MEM_FREE(block->data, char*, block->size);
    MEM_FREE(block, Block*, sizeof(Block));
}

void block_reset(Block* block)
{
    block->data[0] = 0;
    block->next = 1;
}

void block_nest(Block* block)
{
    ++block->data[block->next - 1];

    if (block->next >= block->size) {
        int new_size = block->size * 2;
        MEM_REALLOC(block->data, char*, block->size, new_size);
        block->size = new_size;
    }
    ++block->next;
    block->data[block->next - 1] = 0;

#if defined(BLOCK_DEBUG) && BLOCK_DEBUG > 0
    block_print(block);
#endif
}

void block_exit(Block* block)
{
    --block->next;

#if defined(BLOCK_DEBUG) && BLOCK_DEBUG > 0
    block_print(block);
#endif
}

int block_equals(Block* b1, Block* b2)
{
    // both are identical or null? good!
    if ((b1 == b2) || (!b1 && !b2)) {
        return 1;
    }
    // only one is null? bad!
    if (!b1 || !b2) {
        return 0;
    }
    // both have different nesting? bad!
    if (b1->next != b2->next) {
        return 0;
    }
    // all elements must be identical
    for (int j = 0; j < b1->next; ++j) {
        if (b1->data[j] != b2->data[j]) {
            return 0;
        }
    }
    return 1;
}

int block_contains(Block* b1, Block* b2)
{
    // both are identical or null? good!
    if ((b1 == b2) || (!b1 && !b2)) {
        return 1;
    }
    // only one is null? bad!
    if (!b1 || !b2) {
        return 0;
    }
    // b1 has deeper nesting than b2? it cannot contain it
    if (b1->next > b2->next) {
        return 0;
    }

    // all elements of b1, except the last one, must be identical to
    // corresponding elements in b2
    int top = b1->next - 1;
    for (int j = 0; j < top; ++j) {
        if (b1->data[j] != b2->data[j]) {
            return 0;
        }
    }

    // last element of b1 cannot be greater than corresponding element in
    // b2
    if (b1->data[top] > b2->data[top]) {
        return 0;
    }

    // yes, b1 contains b2
    return 1;
}

int block_format(Block* block, char* buf)
{
    int p = 0;
    for (int j = 0; block && j < block->next; ++j) {
        if (p > 0) {
            buf[p++] = '.';
        }
        p += sprintf(buf + p, "%d", block->data[j]);
    }
    buf[p] = '\0';
    return p;
}

#if defined(BLOCK_DEBUG) && BLOCK_DEBUG > 0
static int block_print(Block* block)
{
    char buf[256];
    block_format(block, buf);
    printf("B [%s]\n", buf);
    return 0;
}
#endif
