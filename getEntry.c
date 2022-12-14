#include "main.h"
#define ENTRYSIZE 1024

uint64_t getEntry(int fd, int entrynum, uint64_t mft_location) {
  // Calculate the offset to the entryNum
  uint64_t entry_offset = entrynum * ENTRYSIZE;
  
  // Calculate the absolute address of this entryNum
  uint64_t entry_location = mft_location + entry_offset;

  return entry_location;
}
