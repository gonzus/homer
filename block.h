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

// Return true if block parent contains block child
int block_contains(Block* parent, Block* child);

// Format a block for printing
int block_format(Block* block, char* buf);

#endif
