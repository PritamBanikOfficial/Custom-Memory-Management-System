#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE 1024 * 1024  // 1MB heap size

typedef struct Block {
    size_t size;
    struct Block* next;
    int is_free;
} Block;

static char heap[HEAP_SIZE];
static Block* free_list = (Block*)heap;

// Function to initialize the heap
void initialize_heap() {
    free_list->size = HEAP_SIZE - sizeof(Block);
    free_list->is_free = 1;
    free_list->next = NULL;
}

// Custom malloc implementation
void* my_malloc(size_t size) {
    Block* current = free_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = 0;
            return (void*)(current + 1);
        }
        current = current->next;
    }
    return NULL;  // No suitable block found
}

// Custom free implementation
void my_free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)ptr - 1; // Get the block metadata
    block->is_free = 1;
}

// Custom realloc implementation
void* my_realloc(void* ptr, size_t size) {
    if (!ptr) return my_malloc(size);

    Block* block = (Block*)ptr - 1;
    if (block->size >= size) return ptr;

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        my_free(ptr);
    }
    return new_ptr;
}

void print_free_list() {
    Block* current = free_list;
    printf("Free List:\n");
    while (current != NULL) {
        printf("Block at %p: Size = %zu, Is Free = %d\n", (void*)current, current->size, current->is_free);
        current = current->next;
    }
}

int main() {
    initialize_heap();

    void* ptr1 = my_malloc(100);
    void* ptr2 = my_malloc(200);

    print_free_list();

    ptr1 = my_realloc(ptr1, 150);
    my_free(ptr1);
    my_free(ptr2);

    print_free_list();

    return 0;
}

