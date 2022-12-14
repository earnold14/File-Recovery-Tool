#include "ntfs.h"
#include "main.h"

#define BUFSIZE 512

// gets logic block address of cluster 0
uint16_t getLBA(int fd) {
  uint8_t mbr_buffer[BUFSIZE];
  MBR* mbr;

  lseek(fd, 0, SEEK_SET);
  // first find start of partition. 
  read(fd, mbr_buffer, BUFSIZE);
  mbr = (MBR*) mbr_buffer;
  
  if (mbr->mbr_sig != 0xAA55) {
    printf("ERROR: Not in MBR\n");
    return 0;
  }

  // lseek to the start of partition. 
  PTE partition0 = mbr->mbr_ptable[0];
  uint16_t lba = partition0.pte_lba;
  uint16_t cluster0 = lba*512;
  
  return cluster0;
}
