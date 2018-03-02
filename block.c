#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "block.h"

static int block_print(Block* block);

Block* block_create(void)
{
    Block* block = (Block*) malloc(sizeof(Block));
    block_reset(block);
    return block;
}

Block* block_clone(Block* block)
{
    if (!block) {
        return 0;
    }
    Block* clone = (Block*) malloc(sizeof(Block));
    memcpy(clone, block, sizeof(Block));
    return clone;
}

void block_destroy(Block* block)
{
    free(block);
}

void block_reset(Block* block)
{
    memset(block, 0, sizeof(Block));
    block->size = 1;
}

void block_down(Block* block)
{
#if 1
    ++block->value[block->size - 1];
    block_print(block);
#endif
}

void block_right(Block* block)
{
    ++block->size;
    block->value[block->size - 1] = 1;
    block_print(block);
}

void block_left(Block* block)
{
    --block->size;
    // block_print(block);
}

int block_contains(Block* parent, Block* child)
{
    // no parent or no child? assumes it is contained
    if (!parent || !child) {
        return 1;
    }

    // parent is more nested than child? it cannot contain it
    if (parent->size > child->size) {
        return 0;
    }

    // all elements of parent, except the last one, must be identical to
    // corresponding elements in child
    int top = parent->size - 1;
    for (int j = 0; j < top; ++j) {
        if (parent->value[j] != child->value[j]) {
            return 0;
        }
    }

    // last element of parent canno be greater than corresponding element in
    // child
    if (parent->value[top] > child->value[top]) {
        return 0;
    }

    // yes, parent contains child
    return 1;
}

int block_format(Block* block, char* buf)
{
    int p = 0;
    for (int j = 0; block && j < block->size; ++j) {
        if (p > 0) {
            *(buf + p++) = '.';
        }
        p += sprintf(buf + p, "%d", block->value[j]);
    }
    *(buf + p) = '\0';
    return p;
}

static int block_print(Block* block)
{
    char buf[256];
    block_format(block, buf);
    printf("B [%s]\n", buf);
    return 0;
}
