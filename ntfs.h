#ifndef NTFS_H
#define NTFS_H

#include <stdint.h>

typedef struct master_file_table_entry {
  struct master_file_table_header {
    // 48B
    uint32_t mft_file;
    uint16_t mft_sequenceoffset;
    uint16_t mft_fixupentries;
    uint64_t mft_lsn;
    uint16_t mft_seqnum;
    uint16_t mft_hlc;
    uint16_t mft_attribute1offset;
    uint16_t mft_flags;
    uint32_t mft_entrysizeused;
    uint32_t mft_entrysizealloc;
    uint64_t mft_fileref;
    uint16_t mft_nextattributeid;
    uint16_t mft_align;
    uint32_t mft_mftnum;
  } MFT_HEADER;
  uint8_t attribute[];
} __attribute__((packed)) MFTE;

typedef struct volume_boot_sector {
  // 25B - BPB
  uint16_t vbr_bps;
  uint8_t vbr_spc;
  uint16_t vbr_res0;
  uint8_t vbr_res1[3];
  uint16_t vbr_res2;
  uint8_t vbr_meddesc;
  uint16_t vbr_res3;
  uint16_t vbr_unused0;
  uint16_t vbr_unused1;
  uint32_t vbr_unused2;
  uint32_t vbr_res4;
   
  // 48B - Extended BPB
  uint32_t vbr_unused3;
  uint64_t vbr_totalsectors;
  uint64_t vbr_mft;
  uint64_t vbr_mftmirr;
  uint32_t vbr_cpfrs;
  uint8_t vbr_cpib;
  uint8_t vbr_unused5[3];
  uint64_t vbr_volserial;
  uint32_t vbr_unused6;
} __attribute__((packed)) VBR;

typedef struct partition_boot_sector {
  uint8_t pbs_jump[3];
  uint8_t pbs_oemid[8];
  uint8_t pbs_vbr[73];
  uint8_t pbs_bootstrap[426];
  uint16_t pbs_sig;
} PBS;

typedef struct partition_table_entry {
  uint8_t pte_flag;
  uint8_t pte_chsbegin[3];
  uint8_t pte_type;
  uint8_t pte_chsend[3]; 
  uint32_t pte_lba;
  uint32_t pte_sectors;
} PTE;

typedef struct master_boot_record {
  uint8_t mbr_code[440];
  uint32_t mbr_disksig;
  uint16_t mbr_nulls;
  PTE mbr_ptable[4];
  uint16_t mbr_sig;
} __attribute__((packed)) MBR;

#endif
