#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "memory.h"
#include "display.h"
#include "bshell.h"
#include "input.h"

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

// moves all elements from index to the right
// trusts that there will be no element overflow
// if there is...
// we lose memory
// FOREVER
// good luck
void move_right(void *array, size_t size, size_t index, size_t element_size) {
  char *arr = (char *)array;
  for (size_t i = size - 1; i > index; i--) {
    //for (size_t j = 0; j < element_size; j++) {
    //  arr[i * element_size + j] = arr[(i - 1) * element_size + j];
    //}
    mem_move(arr + (i - 1) * element_size, arr + i * element_size, element_size);
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

// mem_init(mem_sectors, mm_count)
// we go through all the memory segments and make an array of some struct free_mem that holds the address and size of that memory segment
void mem_init(struct mmap_entry mem_sectors[32], size_t mm_count) {
  mem_set(free_sectors, 0, 32*sizeof(struct free_mem));
  // to change back change size_t i to 0, and mem_sectors free_sectors to i
  for (size_t i = 0; i < mm_count; i++) {
    free_sectors[i].addr = mem_sectors[i].addr;
    free_sectors[i].len = mem_sectors[i].len;
  }
  compress_free_mem(free_sectors, 32);
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

// diabolical naming
size_t last_free_mem_segment() {
  size_t last = 0;
  for (size_t i = 0; i < 32; i++) {
    if (free_sectors[i].len != 0) {
      return last-1;
    }

    last = i;
  }
  return last;
}

// mem_free(ptr, size)
// we go through the array of free memory segments
// if the pointer is at length+addr of the segment we're looking at, we just add size to length of the segment
// if the pointer+len is at the addr of the segment we're looking at, we just add size to length, and minus the size from the addr
// if the pointer > prev.addr+len and pointer+len < current.addr, then the pointer is somewhere between
// so we copy everything from current.addr on to the right and insert the free memory after prev
void mem_free(void *ptr, size_t size) {
  uint64_t addr = (uint64_t)ptr;
  if (addr < free_sectors[0].addr) {
    // if we get here it means the memory is before the first entry
    // which means we need to add an entry before the first entry
    move_right(free_sectors, 32, 0, sizeof(struct free_mem));
    free_sectors[0].len = size;
    free_sectors[0].addr = addr;
    compress_free_mem(free_sectors, 32);
    return;
  }
  for (size_t i = 0; i < 31; i++) {
    if (addr == free_sectors[i].addr+free_sectors[i].len) {
      free_sectors[i].len += size;
      break;
    }
    else if (addr+size == free_sectors[i].addr) {
      free_sectors[i].len += size;
      free_sectors[i].addr -= size;
      break;
    }
    else if (addr > free_sectors[i].addr+free_sectors[i].len && addr+size < free_sectors[i+1].addr && i != 31) {
      move_right(free_sectors, 32, i, sizeof(struct free_mem));
      free_sectors[i + 1].addr = addr;
      free_sectors[i + 1].len = size;
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
void display_mem(bool wait) {
  prints("\nMemory map\n====================================");
  for (size_t i = 0; i < 32; i++) {
    prints("\nMemory number ");
    print_sizet(i);
    prints("\nAddress: ");
    print_uint64(free_sectors[i].addr);
    prints("\nLength: ");
    print_uint64(free_sectors[i].len);

    if (free_sectors[i+1].len == 0) {
      prints("\n\n");
      print_sizet(i+1);
      prints(" free sectors found.\n\n");
      break;
    }

    if (wait) {
      size_t returnval = waitforinput();
      if (returnval == 1) {
        wait = false;
      }
    }
    prints("\n");
  }
}

void list_init(struct list *list, uint32_t size, uint32_t entry_size) {
  list->size = size;
  list->entry_size = entry_size;
  list->item_count = 0;
  list->addr = mem_hold(entry_size * size);
}

void list_push(struct list *list, void *entry) {
  if (list->item_count == list->size) {
    // need to allocate bigger memory
    // for now double the size
    mem_free(list->addr, list->entry_size * list->size);
    list->size *= 2;
    void* old_addr = list->addr;
    list->addr = mem_hold(list->entry_size * list->size);
    if (list->addr = NULL) {
      // not enough memory to double size, we just increase by one
      list->size = (list->size / 2) + 1;
      list->addr = mem_hold(list->entry_size * list->size);
      if (list->addr == NULL) {
        // we actually have no memory left even for one more puny element
        return;
      }
    }
    mem_move(old_addr, list->addr, list->entry_size * list->item_count);
  }
  // add element
  mem_copy(entry, (void *)((uint64_t)(list->addr) + ((uint64_t)(list->item_count) * (uint64_t)(list->entry_size))), list->entry_size);
  list->item_count++;
}

void list_pop(struct list *list) {
  if (list->item_count == 0) {
    return;
  }
  list->item_count--;
  mem_set(list->addr + list->item_count * list->entry_size, 0, list->entry_size);
}

void *list_at(struct list *list, uint32_t index) {
  if (list->item_count == 0) {
    return NULL;
  }
  return list->addr + (index * list->entry_size);
}

void list_insert(struct list *list, uint32_t index, void *entry) {
  if (list->item_count == list->size) {
    // we need to allocate more memory
    // let's just use the same function as in list_push
    // it's gonna get overwriten anyway so who cares what entry gets appended
    // also it increases the item_count by 1 for us
    list_push(list, entry);
  }
  move_right(list->addr, list->item_count, index, list->entry_size);
  mem_copy(entry, list->addr + index * list->entry_size, list->entry_size);
}

void list_remove(struct list *list, uint32_t index) {
  move_left(list->addr, list->item_count, index, list->entry_size);
  list_pop(list);
  list->item_count--;
}

void list_empty(struct list *list) {
  mem_set(list->addr, 0, list->entry_size * list->size);
  list->item_count = 0;
}

void display_list(struct list *list) {
  prints("List size: ");
  print_sizet(list->size);
  prints("\nList entry size: ");
  print_sizet(list->entry_size);
  prints("\nList item count: ");
  print_sizet(list->item_count);
  prints("\nList address: ");
  print_uint64((uint64_t)list->addr);
  prints("\n\n");
  for (size_t i = 0; i < list->item_count; i++) {
    prints("Item ");
    print_sizet(i);
    prints(": ");
    prints((char*)list->addr + i * list->entry_size);
    prints("\n");
  }
}

// file_system holds the address of a list of addresses of files, and the address of a list of addresses of workspaces
void init_filesystem(struct file_system *fs) {
  void *files = mem_hold(sizeof(struct list));
  void *workspaces = mem_hold(sizeof(struct list));
  list_init(files, 1, sizeof(struct file));
  list_init(workspaces, 1, sizeof(struct workspace));
  fs->files = files;
  fs->workspaces = workspaces;
}
