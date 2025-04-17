#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MEMORY_SIZE 1024  // Total memory size
#define MIN_BLOCK_SIZE 16 // Smallest allocatable block size

typedef struct Block {
    size_t size;
    int free;
    struct Block *next;
} Block;

int levels;              // Number of levels in the buddy system
Block **free_list;       // Array of linked lists storing free blocks

void initialize_memory() {
    levels = (int)log2(MEMORY_SIZE / MIN_BLOCK_SIZE) + 1;
    free_list = (Block **)malloc(levels * sizeof(Block *));
    
    if (!free_list) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    for (int i = 0; i < levels; i++) {
        free_list[i] = NULL;
    }

    // Initialize a single large free block
    Block *initial_block = (Block *)malloc(sizeof(Block));
    initial_block->size = MEMORY_SIZE;
    initial_block->free = 1;
    initial_block->next = NULL;

    free_list[levels - 1] = initial_block; // Place it in the highest level

    printf("Buddy system initialized with %d levels.\n", levels);
}

// Find the smallest level that can fit the requested size
int find_level(size_t size) {
    int level = 0;
    size_t block_size = MIN_BLOCK_SIZE;
    while (block_size < size) {
        level++;
        block_size *= 2;
    }
    return level;
}

// Split a larger block into two buddies
void split_block(int level) {
    if (level >= levels - 1 || free_list[level + 1] == NULL) return;

    Block *block = free_list[level + 1];
    free_list[level + 1] = block->next;

    size_t half_size = block->size / 2;
    
    Block *buddy1 = (Block *)malloc(sizeof(Block));
    buddy1->size = half_size;
    buddy1->free = 1;
    buddy1->next = NULL;

    Block *buddy2 = (Block *)malloc(sizeof(Block));
    buddy2->size = half_size;
    buddy2->free = 1;
    buddy2->next = NULL;

    buddy1->next = buddy2;
    buddy2->next = free_list[level];

    free_list[level] = buddy1;
}

// Allocate memory of a given size
Block *allocate(size_t size) {
    int level = find_level(size);
    
    for (int i = level; i < levels; i++) {
        if (free_list[i] != NULL) {
            while (i > level) {
                split_block(i - 1);
                i--;
            }
            Block *allocated = free_list[level];
            free_list[level] = allocated->next;
            allocated->free = 0;
            printf("Allocated %zu bytes at level %d.\n", allocated->size, level);
            return allocated;
        }
    }
    
    printf("Allocation failed! Not enough memory.\n");
    return NULL;
}

// Merge adjacent free blocks (buddies)
void merge_buddies(int level) {
    if (level >= levels - 1) return;

    Block *current = free_list[level];
    while (current && current->next) {
        if (current->size == current->next->size) {
            Block *buddy = current->next;
            current->size *= 2;
            current->next = buddy->next;
            free(buddy);
        }
        current = current->next;
    }
}

// Free allocated memory
void free_memory(Block *block) {
    if (!block || block->free) {
        printf("Invalid or double free detected!\n");
        return;
    }

    int level = find_level(block->size);
    block->free = 1;
    block->next = free_list[level];
    free_list[level] = block;
    
    merge_buddies(level);
    printf("Freed %zu bytes at level %d.\n", block->size, level);
}

// Display current memory status
void display_memory() {
    printf("\nCurrent Memory Status:\n");
    for (int i = 0; i < levels; i++) {
        printf("Level %d (Block size %d): ", i, (int)pow(2, i + log2(MIN_BLOCK_SIZE)));
        Block *current = free_list[i];
        while (current) {
            printf("[%zu] -> ", current->size);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main() {
    initialize_memory();

    int choice;
    size_t size;
    Block *allocated_blocks[10] = {NULL}; // Track allocated blocks for freeing

    do {
        printf("\nMenu:\n");
        printf("1. Allocate Memory\n");
        printf("2. Free Memory\n");
        printf("3. Display Memory\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter size to allocate: ");
                scanf("%zu", &size);
                for (int i = 0; i < 10; i++) {
                    if (!allocated_blocks[i]) {
                        allocated_blocks[i] = allocate(size);
                        break;
                    }
                }
                break;

            case 2:
                printf("Enter block index to free (0-9): ");
                int index;
                scanf("%d", &index);
                if (index >= 0 && index < 10 && allocated_blocks[index]) {
                    free_memory(allocated_blocks[index]);
                    allocated_blocks[index] = NULL;
                } else {
                    printf("Invalid block index!\n");
                }
                break;

            case 3:
                display_memory();
                break;

            case 4:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);

    return 0;
}
