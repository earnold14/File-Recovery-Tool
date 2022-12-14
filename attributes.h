#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <stdint.h>

typedef struct standard_header {
  uint32_t std_attributeType;
  uint32_t std_totalLength;
  uint8_t std_nonresFlag;
  uint8_t std_nameLength; // N
  uint16_t std_nameOffset;
  uint16_t std_flags;
  uint16_t std_attributeID;
} STD_HEADER;


typedef struct resident_noname {
  // Resident, No Name
  uint32_t resnn_attributeLength; // L
  uint16_t resnn_attributeOffset; 
  uint8_t resnn_indexedFlags;
  uint8_t resnn_padding;
  uint8_t resnn_attribute[1]; // = L
} __attribute__((packed)) RES_NN;

typedef struct resident_named {
  // Resident, Named
  uint32_t resn_attributeLength; // L
  uint16_t resn_attributeOffset; 
  uint8_t resn_indexedFlags;
  uint8_t resn_padding;
  uint16_t resn_attributeName[1]; // = 2N
  uint8_t resn_attribute[1]; // = 2N + 0x18   rounded up to multiple of 4 bytes
} __attribute__((packed)) RES_N;

typedef struct nonresident_noname {
  // Non-Resident, No Name
  uint64_t nresnn_startingVCN;
  uint64_t nresnn_lastVCN;
  uint16_t nresnn_datarunOffset;
  uint16_t nresnn_compressionUnitSize;
  uint32_t nresnn_padding;
  uint64_t nresnn_attributeAllocatedSize;
  uint64_t nresnn_attributeRealSize;
  uint64_t nresnn_initializedSize;

  unsigned int data_clusterOffsetSize : 4;
  unsigned int data_clusterLengthSize : 4;
  uint64_t data_clusterSize[1];
  uint64_t data_clusterOffset[1];

} __attribute__((packed)) NRES_NN;

typedef struct nonresident_named {
  // Non-Resident, Named
  uint64_t nresn_startingVCN;
  uint64_t nresn_lastVCN;
  uint16_t nresn_datarunOffset;
  uint16_t nresn_compressionUnitSize;
  uint32_t nresn_padding;
  uint64_t nresn_attributeAllocatedSize;
  uint64_t nresn_attributeRealSize;
  uint64_t nresn_initializedSize;
  uint8_t nresn_attributeName[1]; // 2N

  unsigned int data_clusterOffsetSize : 4;
  unsigned int data_clusterLengthSize : 4;
  uint64_t data_clusterSize[1];
  uint64_t data_clusterOffset[1];
} __attribute__((packed)) NRES_N;

  
typedef struct name_attribute {
  // 24B
  STD_HEADER name_standardHeader;

  struct name_header {
    // 66B + payload
    uint64_t name_pEntryNum:48;
    uint16_t name_pSequenceNum;
    uint64_t name_fileCreated;
    uint64_t name_fileModified;
    uint64_t name_recordChanged;
    uint64_t name_fileAccessed;
    uint64_t name_sizeAlloc;
    uint64_t name_sizeReal;
    uint32_t name_nFlags;
    uint32_t name_reparse;
    uint8_t name_fileLength;
    uint8_t name_fileNamespace;
    unsigned char name_payload[1];
  } __attribute__((packed)) NAME_HEADER;
} __attribute__((packed)) NAME;

#endif
