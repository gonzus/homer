#ifndef BLOCK_H_
#define BLOCK_H_

typedef struct Block {
    int size;
    char value[32];
} Block;

Block* block_create(void);
Block* block_clone(Block* block);
void block_destroy(Block* block);

void block_reset(Block* block);

void block_down(Block* block);
void block_right(Block* block);
void block_left(Block* block);

int block_contains(Block* parent, Block* child);

int block_format(Block* block, char* buf);

#endif
