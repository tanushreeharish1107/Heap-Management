#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_SIZE 1024  

typedef struct Block {
    size_t size;
    bool is_free;
    struct Block *next;  // For free list
    struct Block *allocated_next;  // For tracking allocated blocks
} Block;

static char heap[HEAP_SIZE];
static Block *free_list = (Block *)heap;
static Block *allocated_list = NULL;  // New list for allocated blocks

void init_heap() {
    free_list->size = HEAP_SIZE - sizeof(Block);
    free_list->is_free = true;
    free_list->next = NULL;
    allocated_list = NULL;  
}

//  Add to Allocated List
void add_to_allocated_list(Block *block) {
    block->allocated_next = allocated_list;
    allocated_list = block;
}

// Remove from Allocated List
void remove_from_allocated_list(Block *block) {
    Block **current = &allocated_list;
    while (*current) {
        if (*current == block) {
            *current = block->allocated_next; // Remove from list
            return;
        }
        current = &((*current)->allocated_next);
    }
}

//  Allocate Memory
void *allocate(size_t size) {
    Block *current = free_list;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            size_t remaining = current->size - size - sizeof(Block);
            
            if (remaining > sizeof(Block)) {  
                Block *new_block = (Block *)((char *)current + sizeof(Block) + size);
                new_block->size = remaining;
                new_block->is_free = true;
                new_block->next = current->next;
                
                current->size = size;
                current->is_free = false;
                current->next = new_block;
            } else {
                current->is_free = false;
            }

            add_to_allocated_list(current);  // Track allocated block
            return (void *)((char *)current + sizeof(Block));
        }
        current = current->next;
    }
    return NULL;
}

// Merge Adjacent Free Blocks
void merge_blocks() {
    Block *current = free_list;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// Free Memory
void free_memory(void *ptr) {
    if (!ptr) return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));

    // Check if block is in the allocated list
    Block *check = allocated_list;
    bool found = false;
    while (check) {
        if (check == block) {
            found = true;
            break;
        }
        check = check->allocated_next;
    }

    if (!found) {
        printf("Error: Attempting to free unallocated memory!\n");
        return;
    }

    remove_from_allocated_list(block);  // Remove from allocated list
    block->is_free = true;
    merge_blocks();
}

// Display Memory Status
void display_memory() {
    Block *current = free_list;
    printf("\nHeap Status:\n");
    
    while (current) {
        printf("[Size: %zu, %s] -> ", current->size, current->is_free ? "Free" : "Allocated");
        current = current->next;
    }
    printf("NULL\n");

    printf("\nAllocated Blocks:\n");
    Block *alloc = allocated_list;
    while (alloc) {
        printf("[Size: %zu] -> ", alloc->size);
        alloc = alloc->allocated_next;
    }
    printf("NULL\n");
}


int main() {

    init_heap();
    
    printf("Initial Heap:\n");
    display_memory();

    void *p1 = allocate(200);
    void *p2 = allocate(300);
    void *p3 = allocate(100);
    
    printf("\nAfter Allocations:\n");
    display_memory();

    free_memory(p2);
    printf("\nAfter Freeing p2:\n");
    display_memory();

    void *p4 = allocate(250);
    printf("\nAfter Allocating p4 (250 bytes):\n");
    display_memory();

    free_memory(p1);
    free_memory(p3);
    free_memory(p4);
    
    printf("\nAfter Freeing All Blocks:\n");
    display_memory();

    
    printf("\nTesting Safety Check:\n");
    free_memory(p2);  // This was already freed, should print an error.

    return 0;
   


   
}
