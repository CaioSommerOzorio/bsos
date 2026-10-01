#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "memory.h"
#include "display.h"
#include "bshell.h"

struct free_mem free_sectors[32];

void mem_set(void* ptr, int value, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((char*)ptr)[i] = value;
  }
}

void mem_copy(const void* src, void* dest, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}

void mem_move(void* src, void* dest, size_t size) {
  mem_copy(src, dest, size);
  mem_set(src, 0, size);
}

void meminit(void *memory_sector) {
  mem_set(memory_sector, 0, 4096);
}

// moves all elements from index to the right
// trusts that there will be no element overflow
// if there is...
// we lose memory
// FOREVER
// good luck
void move_right(void *array, size_t size, size_t index, size_t element_size) {
  char *arr = (char *)array;
  for (size_t i = size - 1; i > index; i--) {
    for (size_t j = 0; j < element_size; j++) {
      arr[i * element_size + j] = arr[(i - 1) * element_size + j];
    }
  }
}

// moves everything from the right of the index one spot to the left
// element at index will get deleted
void move_left(void *array, size_t size, size_t index, size_t element_size) {
  char *arr = (char *)array;
  for (size_t i = index; i < size - 1; i++) {
    mem_move(arr + (i + 1) * element_size, arr + i * element_size, element_size);
  }
}

// compress_free_mem()
// this will find any free memory segments that are adjacent and merge them
void compress_free_mem(struct free_mem *free_mem, size_t size) {
  display_mem();
  for (size_t i = 0; i < size - 1; ) {
    if (free_mem[i].addr + free_mem[i].len == free_mem[i + 1].addr && free_mem[i].len != 0) {
      free_mem[i].len += free_mem[i + 1].len;
      move_left(free_mem, size, i + 1, sizeof(struct free_mem));
      size--;
    }
    else {
      i++;
    }
  }
}

// mem_init(mem_secotrs, mm_count)
// we go through all the memory segments and make an array of some struct free_mem that holds the address and size of that memory segment
void mem_init(struct mmap_entry mem_sectors[32], size_t mm_count) {
  mem_set(free_sectors, 0, 32*sizeof(struct free_mem));
  for (size_t i = 0; i < mm_count; i++) {
    free_sectors[i].addr = mem_sectors[i].addr;
    free_sectors[i].len = mem_sectors[i].len;
  }
}

// mem_hold(size) -> returns pointer to the memory segment
// we look at the first entry in the array of memory segments
// if it's long enough, we change its addr to addr+size, and write return the addr.
// otherwise we look at the next entry, rinse and repeat
void *mem_hold(size_t size) {
  for (size_t i = 0; i < 32; i++) {
    if (free_sectors[i].len >= size) {
      free_sectors[i].addr += size;
      free_sectors[i].len -= size;
      if (free_sectors[i].len == 0) {
        move_left(free_sectors, 32, i, sizeof(struct free_mem));
      }
      return (void *)free_sectors[i].addr-size;
    }
  }
  // will comment this out later but probably good for debugging
  prints("out of memory lol\n");
  return NULL;
}

// mem_free(ptr, size)
// we go through the array of free memory segments
// if the pointer is at length+addr of the segment we're looking at, we just add size to length of the segment
// if the pointer+len is at the addr of the segment we're looking at, we just add size to length, and minus the size from the addr
// if the pointer > prev.addr+len and pointer+len < current.addr, then the pointer is somewhere between
// so we copy everything from current.addr on to the right and insert the free memory after prev
void mem_free(void *ptr, size_t size) {
  if ((uint64_t) ptr <= free_sectors[0].addr) {
    // if we get here it means the memory is before the first entry
    // which means we need to add an entry before the first entry
    move_right(free_sectors, 32, 0, sizeof(struct free_mem));
    free_sectors[0].len = size;
    free_sectors[0].addr = (uint64_t)ptr;
    compress_free_mem(free_sectors, 32);
    return;
  }
  for (size_t i = 0; i < 31; i++) {
    if ((uint64_t)ptr == free_sectors[i].addr+free_sectors[i].len) {
      free_sectors[i].len += size;
      break;
    }
    else if ((uint64_t)ptr+size == free_sectors[i].addr) {
      free_sectors[i].len += size;
      free_sectors[i].addr -= size;
      break;
    }
    else if (ptr > free_sectors[i].addr+free_sectors[i].len && ptr+size < free_sectors[i+1].addr && i != 31) {
      // will get rid of this later but for now
      // good for debugging
      if (i == 31) {
        prints("\nout of memory\n");
        break;
      }
      move_right(free_sectors, 32, i, sizeof(struct free_mem));
      free_sectors[i].addr = (uint64_t)ptr;
      free_sectors[i].len = size;
      break;
    }
  }
  compress_free_mem(free_sectors, 32);
}

// returns number of usable memory segments
size_t parse_mmap(void *mb_info, struct mmap_entry *usable_mem, size_t max_entries) {
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
      while ((char*)mmap < (char *)tag + tag->size && i < max_entries) {
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


// visualizes free memory sectors
void display_mem(void) {
  for (size_t i = 0; i < 32; i++) {
    if (free_sectors[i].len == 0) {
      break;
    }
    prints("Memory number ");
    print_sizet(i);
    prints(": \nAddress: ");
    print_uint64(free_sectors[i].addr);
    prints("\nLength: ");
    print_uint64(free_sectors[i].len);
    prints("\n\n");
    waitforinput();
  }
}
