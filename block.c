#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "block.h"

#define BLOCK_DEBUG 0
#define BLOCK_DEFAULT_SIZE 32

#if defined(BLOCK_DEBUG) && BLOCK_DEBUG > 0
static int block_print(Block* block);
#endif

Block* block_create(int size)
{
    Block* block = (Block*) malloc(sizeof(Block));
    block->size = size <= 0 ? BLOCK_DEFAULT_SIZE : size;
    block->data = (char*) malloc(block->size);
    block_reset(block);
    return block;
}

Block* block_clone(Block* block)
{
    if (!block) {
        return 0;
    }
    Block* clone = (Block*) malloc(sizeof(Block));
    clone->size = block->size;
    clone->next = block->next;
    clone->data = (char*) malloc(clone->size);
    memcpy(clone->data, block->data, clone->next);
    return clone;
}

void block_destroy(Block* block)
{
    LOG(("BLOCK FREE %d", block->size));
    free(block->data);
    free(block);
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
        LOG(("BLOCK GROW %d -> %d", block->size, new_size));
        char* new_data = realloc(block->data, new_size);
        block->data = new_data;
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

int block_contains(Block* parent, Block* child)
{
    // no parent or no child? assumes it is contained
    if (!parent || !child) {
        return 1;
    }

    // parent has deeper nesting than child? it cannot contain it
    if (parent->next > child->next) {
        return 0;
    }

    // all elements of parent, except the last one, must be identical to
    // corresponding elements in child
    int top = parent->next - 1;
    for (int j = 0; j < top; ++j) {
        if (parent->data[j] != child->data[j]) {
            return 0;
        }
    }

    // last element of parent cannot be greater than corresponding element in
    // child
    if (parent->data[top] > child->data[top]) {
        return 0;
    }

    // yes, parent contains child
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
