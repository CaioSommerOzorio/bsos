#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <display.h>

struct mb_tag {
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
};

struct mmap_entry {
  uint64_t addr;
  uint64_t len;
  uint32_t type;
  uint32_t zero;
};

struct free_mem {
  uint64_t addr;
  uint64_t len;
};

struct list {
  uint32_t size;
  uint32_t entry_size;
  uint32_t item_count;
  uint64_t *addr;
};

size_t parse_mmap(void *mb_info, struct mmap_entry *usable_mem, size_t max_entries);
void move_right(void *array, size_t size, size_t index, size_t element_size);
void move_left(void *array, size_t size, size_t index, size_t element_size);
void compress_free_mem(struct free_mem *mem, size_t size);
void mem_init(struct mmap_entry mem_sectors[32], size_t mm_count);
void *mem_hold(size_t size);
void mem_free(void *ptr, size_t size);
void mem_set(void* ptr, int value, size_t size);
void mem_copy(const void* src, void* dest, size_t size);
void mem_move(void* src, void* dest, size_t size);
void display_mem(bool wait);
void list_init(struct list *list, uint32_t size, uint32_t entry_size);
void list_push(struct list *list, void *entry);
void list_pop(struct list *list);
void *list_at(struct list *list, uint32_t index);
void list_insert(struct list *list, uint32_t index, void *entry);

#endif
