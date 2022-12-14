#include <string.h>
#include "ntfs.h"
#include "attributes.h"
#include "main.h"

#define ENTRYSIZE 1024

uint64_t getAttributeLocation(MFTE* entry, uint64_t entry_location, uint32_t attrib_name);
void directoryCheck(MFTE* entry);


uint64_t traverseAttribute(int fd, uint64_t entry_location, uint32_t attrib_name) {
  uint8_t entry_buffer[ENTRYSIZE];
  uint16_t entry_offset;
  MFTE* entry2 = malloc(ENTRYSIZE);

  // lseek to offset and read buffer
  lseek(fd, entry_location, SEEK_SET);
  read(fd, entry_buffer, ENTRYSIZE);
  entry2 = (MFTE*) entry_buffer;

  entry_offset = entry2->MFT_HEADER.mft_attribute1offset;
  memcpy(entry2->attribute, entry_buffer + (uintptr_t) entry_offset, (uintptr_t) ENTRYSIZE - (uintptr_t) entry_offset);

  directoryCheck(entry2);

  // returns location of attribute
  return getAttributeLocation(entry2, entry_location, attrib_name);
}

// Traverse through attributes
uint64_t getAttributeLocation(MFTE* entry, uint64_t entry_location, uint32_t attrib_name) {

  int offset = 0;
  uint32_t* a_name2 = malloc(4*sizeof(uint8_t));
  uint32_t* a_len2 = malloc(4*sizeof(uint8_t));
  uint32_t a_name = 0;
  uint32_t a_len = 0;  
  uint16_t entry_offset = entry->MFT_HEADER.mft_attribute1offset;

  // Stops traversal when empty
  while (a_name != 0xFFFFFFFF) {
    uint64_t attrib_location = entry_location + offset + entry_offset;
    // attribute name
    memcpy(a_name2, entry->attribute + offset, 4 * sizeof(uint16_t));
    // attribute length
    memcpy(a_len2, entry->attribute + offset + 4, 4 * sizeof(uint16_t));
    a_name = *a_name2;
    a_len = *a_len2;

    if (a_name == 0x00000030 || a_name == 0x00000080)
      //      printf("Attribute: 0x%08X\tLocation: 0x%08lX\n",a_name,attrib_location);
    
    // Traverse to specified attribute
    if (a_name == attrib_name) {
      return attrib_location;
    }

    // Traverses to next attribute
    offset = offset + a_len;
  }
  return 0;
}

// Check if file is a directory
void directoryCheck(MFTE* entry) {
  switch(entry->MFT_HEADER.mft_flags) {
  case 0x00:
    printf("File not in use\n");
    break;
  case 0x01:
    printf("File in use\n");
    break;
  case 0x02:
    printf("Directory not in use\n");
    break;
  case 0x03:
    printf("Directory in use\n");
    break;
  }
}
