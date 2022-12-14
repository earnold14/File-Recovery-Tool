#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct file_cluster {
  uint64_t attrib_size;
  uint64_t cluster_size;
  uint64_t cluster_offset;
} __attribute__((packed)) FCLUST;

uint64_t getMFT(int fd);
uint16_t getLBA(int fd);
uint16_t getCNUM(int fd);
uint64_t getEntry(int fd, int entrynum, uint64_t mft_location);
uint64_t traverseAttribute(int fd, uint64_t entry_location, uint32_t attrib_name);
FCLUST printData(int fd, uint64_t data_location);
#endif
