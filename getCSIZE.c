#include "ntfs.h"
#include "main.h"

#define BUFSIZE 512

// gets the size of each cluster
uint16_t getCNUM(int fd) {
  uint8_t mbr_buffer[BUFSIZE];
  MBR* mbr;

  
  // first find start of partition. 
  lseek(fd, 0, SEEK_SET);
  read(fd, mbr_buffer, BUFSIZE);
  mbr = (MBR*) mbr_buffer;

  if (mbr->mbr_sig != 0xAA55) {
    printf("ERROR: Not in MBR\n");
    return 0;
  }

  // lseek to the start of partition. 
  PTE partition0 = mbr->mbr_ptable[0];
  uint8_t pbs_buffer[BUFSIZE];
  uint8_t vbr_buffer[73];
  uint16_t lba = partition0.pte_lba;
  PBS* pbs;
  VBR* vbr;
  
  lseek(fd, lba*512, SEEK_SET);
  read(fd, pbs_buffer, BUFSIZE);
  pbs = (PBS*) pbs_buffer;
  vbr = (VBR*) pbs->pbs_vbr;
  
  // find address of MFT
  // confirm the partition is NTFS (signature verification)
  if (pbs->pbs_sig != 0xAA55) {
    printf("ERROR: Not NTFS Partition\n");
    return 0;
  }
  
  // calculating cluster size
  uint16_t bps = vbr->vbr_bps;
  uint8_t spc = vbr->vbr_spc;
  uint16_t cluster_num = bps * spc;

  return cluster_num;
}
