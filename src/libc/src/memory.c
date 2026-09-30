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

// returns number of usable memory segments
size_t parse_mmap(void *mb_info, struct mmap_entry *usable_mem, size_t max_entries) {
  // will return usable_mem
  size_t i = 0;

  // find first tag
  struct mb_tag *tag =
    (struct mb_tag*)((char *)mb_info + 8);

  // look for memory map
  while (tag->type != 0) {
    // memory map found
    if (tag->type == 6) {
      // declare first entry
      struct mmap_entry *mmap =
        (struct mmap_entry *)((char *)tag+16);

      // loop through entries
      while ((char*)mmap < (char *)tag + tag->size) {
        // usable memory found
        if (mmap->type == 1) {
          // add entry to usable memory array
          usable_mem[i] = *mmap;
          i++;
        }

        // move onto next memory map entry
        mmap =
          (struct mmap_entry *)((char *)mmap + tag->entry_size);
      }
    }

    // move onto next tag
    tag =
      (struct mb_tag *)((char *)tag + ((tag->size + 7) & ~7));
  }
  return i;
}

void memset(void* ptr, int value, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((char*)ptr)[i] = value;
  }
}

void memcpy(const void* src, void* dest, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}

void meminit() {}
