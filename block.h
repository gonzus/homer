#ifndef BLOCK_H_
#define BLOCK_H_

typedef struct Block {
    int size;
    int next;
    char* data;
} Block;

Block* block_create(int size);
Block* block_clone(Block* block);
void block_destroy(Block* block);

// Reset a block so that it is empty
void block_reset(Block* block);

// Update a block so that it nests: enters a block
void block_nest(Block* block);

// Update a block so that it exits a block
void block_exit(Block* block);

// Return true if block b1 is the same as block b2
int block_equals(Block* b1, Block* b2);

// Return true if block b1 contains block b2
int block_contains(Block* b1, Block* b2);

// Format a block for printing
int block_format(Block* block, char* buf);

#endif
