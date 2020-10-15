Allocator of memory on C++

The class allocator which can select data in heap, with size that you say. I try to use minimum of variables, so you can see there many operations with pointers and access to them. Memory is divided on blocks, every block contains HEADER and memory. HEADER has fixed size 8 bytes. That mean if need to allocate 8 bytes of memory actually needs 16 bytes free memory in one block. HEADER consist of:

    1 byte - is bool, and show is this block free.
    2 bytes - is size of whole block.
    2 bytes - is size of previous block.
    3 bytes - empty, they actually for leveling,you can use them on your own.

SIZE_OF_MEMORY - it is how many bytes reserved for your needs. Default 1024 bytes or 1 KB. Size of HEADER you can change for your needs. All blocks are leveling by 4. That mean if we need 5 bytes in will up to 8.

Functions:

void* mem_alloc(size_t size) – search a free block with enought memory(don't forget that size we need is HEADER + memory that you give). When finds it a new create block of given size and return pointer to the memory you can use. If in block more memory that we need devide it on 2 blocks. If there is no block with enough memory return nullptr.

void mem_free(void* addr) – mark this block as free. If left or right blocks free so, then unites them.

void* mem_realloc(void* addr, size_t size) – change size of chosen block, can make it smaller or bigger(if neighboring blocks access to do it). Old memory moves to new block. If not enough memory in this block with free adjacent blocks then work as mem_alloc. If there not enough memory for moving of old block returns nullptr. If change of size successful returns pointer to the new block with old data. If addr = nullptr work as mem_alloc.
Usege

To use the algorithm, open the file in any IDE with C++ support. Describe your use cases in main() function or implement it in separate function, then calling it in main() function.

Examples

First example is creation of simple allocator.

Allocator allocator;
    allocator.init(140);
    allocator.mem_dump();
    
    