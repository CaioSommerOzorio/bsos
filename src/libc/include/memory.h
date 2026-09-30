#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

//size_t nof_usable_mem_segments(struct mmap_entry *mm_entry, uint32_t entry_size, uint32_t tag_size);
size_t parse_mmap(void *mb_info, struct mmap_entry *usable_mem, size_t max_entries);
void memset(void* ptr, int value, size_t size);
void memcpy(const void* src, void* dest, size_t size);

#endif
