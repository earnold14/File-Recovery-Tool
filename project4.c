#include "ntfs.h"
#include "attributes.h"
#include "main.h"
#include <math.h>
#include <string.h>

FCLUST res_noname(STD_HEADER* stdAttr, int fd, uint64_t data_location);
FCLUST res_named(STD_HEADER* stdAttr, int fd, uint64_t data_location);
FCLUST nonres_noname(STD_HEADER* stdAttr, int fd, uint64_t data_location);
FCLUST nonres_named(STD_HEADER* stdAttr, int fd, uint64_t data_location);

FCLUST printData(int fd, uint64_t data_location) {
  // navigate to DATA attribute and read standard attribute header
  uint8_t stdAttr_buffer[16];
  lseek(fd, data_location, SEEK_SET);
  read(fd, stdAttr_buffer, 16);
  STD_HEADER* stdAttr;
  stdAttr = (STD_HEADER*) stdAttr_buffer;
  
  //  printf("\n");
  // determining if attribute is resident/named
  if (stdAttr->std_nonresFlag == 0) {
    if (stdAttr->std_nameLength == 0)
      return res_noname(stdAttr, fd, data_location);
    else
      return res_named(stdAttr, fd, data_location);
  }
  else {
    if (stdAttr->std_nameLength == 0)
      return nonres_noname(stdAttr, fd, data_location);
    else
      return nonres_named(stdAttr, fd, data_location);
  }
}



/* resident no name */
FCLUST res_noname(STD_HEADER* stdAttr, int fd, uint64_t data_location) {
  uint8_t dataAttr_buffer[stdAttr->std_totalLength-16];
  lseek(fd, data_location + 16, SEEK_SET);
  read(fd, dataAttr_buffer, stdAttr->std_totalLength-16);
  RES_NN* dataAttr = malloc(stdAttr->std_totalLength-16);      
  dataAttr = (RES_NN*) dataAttr_buffer;
    
  memcpy(dataAttr->resnn_attribute, dataAttr_buffer + (uintptr_t) dataAttr->resnn_attributeOffset, dataAttr->resnn_attributeLength);

  printf("RESIDENT\n");

  FCLUST cluster_data;
  cluster_data = (FCLUST){.attrib_size = 0, .cluster_size = 0, .cluster_offset = 0};
  return cluster_data;
}


  
/* resident named */
FCLUST res_named(STD_HEADER* stdAttr, int fd, uint64_t data_location) {
  uint8_t dataAttr_buffer[stdAttr->std_totalLength-16];
  lseek(fd, data_location + 16, SEEK_SET);
  read(fd, dataAttr_buffer, stdAttr->std_totalLength-16);
  RES_N* dataAttr = malloc(stdAttr->std_totalLength-16);
  dataAttr = (RES_N*) dataAttr_buffer;

  memcpy(dataAttr->resn_attributeName, dataAttr_buffer + (uintptr_t) stdAttr->std_nameOffset, stdAttr->std_nameLength);
  memcpy(dataAttr->resn_attribute, dataAttr_buffer + (uintptr_t) dataAttr->resn_attributeOffset, dataAttr->resn_attributeLength);

  printf("RESIDENT\n");

  FCLUST cluster_data;
  cluster_data = (FCLUST){.attrib_size = 0, .cluster_size = 0, .cluster_offset = 0};
  return cluster_data;
}
 


/* nonresident no name */
FCLUST nonres_noname(STD_HEADER* stdAttr, int fd, uint64_t data_location) {
  uint8_t dataAttr_buffer[stdAttr->std_totalLength-16];
  lseek(fd, data_location + 16, SEEK_SET);
  read(fd, dataAttr_buffer, stdAttr->std_totalLength-16);
  NRES_NN* dataAttr = malloc(stdAttr->std_totalLength-16);
  dataAttr = (NRES_NN*) dataAttr_buffer;
  
    
  // Calculating cluster offset
  long cOffset = 0;
  for (int i = dataAttr->data_clusterOffsetSize; i > 0; i--) {
    int p = pow(0x100,i-1);
    cOffset = cOffset + (p * dataAttr_buffer[dataAttr->nresnn_datarunOffset-16+i]);
  }  

  // Calculating cluster size
  long cSize = 0;
  for (int i = dataAttr->data_clusterLengthSize; i > 0; i--) {
    int p = pow(0x100,i-1);
    cSize = cSize + (p * dataAttr_buffer[dataAttr->data_clusterOffsetSize + dataAttr->nresnn_datarunOffset-16+i]);
    // Checking for a negative number
    if (i == 1 && dataAttr_buffer[dataAttr->data_clusterOffsetSize + dataAttr->nresnn_datarunOffset-16+i] == (0x80 || 0x88))
      cSize |= 0xff << cSize;
  }

  
  // Printing attribute details  
  printf("Non-Resident\n");
  //  printf("Actual size of file: %ld\n",dataAttr->nresnn_attributeRealSize);
  //  printf("Cluster Size: 0x%lX\tCluster Offset: 0x%08lX\n",cOffset,cSize);

  FCLUST cluster_data;
  cluster_data = (FCLUST){.attrib_size = dataAttr->nresnn_attributeRealSize, .cluster_size = cOffset, .cluster_offset = cSize};
  return cluster_data;
}

 

/* nonresident named */
FCLUST nonres_named(STD_HEADER* stdAttr, int fd, uint64_t data_location) {
  uint8_t dataAttr_buffer[stdAttr->std_totalLength-16];
  lseek(fd, data_location + 16, SEEK_SET);
  read(fd, dataAttr_buffer, stdAttr->std_totalLength-16);
  NRES_N* dataAttr = malloc(stdAttr->std_totalLength-16);
  dataAttr = (NRES_N*) dataAttr_buffer;

  // Setting attributes
  memcpy(dataAttr->nresn_attributeName, dataAttr_buffer + (uintptr_t) stdAttr->std_nameOffset, stdAttr->std_nameLength);
  
  // Calculating cluster offset
  long cOffset = 0;
  for (int i = dataAttr->data_clusterOffsetSize; i > 0; i--) {
    int p = pow(0x100,i-1);
    cOffset = cOffset + (p * dataAttr_buffer[dataAttr->nresn_datarunOffset-16+i]);
  }  

  // Calculating cluster size
  long cSize = 0;
  for (int i = dataAttr->data_clusterLengthSize; i > 0; i--) {
    int p = pow(0x100,i-1);
    cSize = cSize + (p * dataAttr_buffer[dataAttr->data_clusterOffsetSize + dataAttr->nresn_datarunOffset-16+i]);

    // Checking for a negative number
    if (i == 1 && dataAttr_buffer[dataAttr->data_clusterOffsetSize + dataAttr->nresn_datarunOffset-16+i] == (0x80 || 0x88))
      cSize |= 0xff << cSize;
  }

  // Printing attribute details  
  printf("Non-Resident\n");
  //  printf("Actual size of file: %ld\n",dataAttr->nresn_attributeRealSize);
  //  printf("Cluster Offset: 0x%lX\tCluster Size: 0x%08lX\n",cOffset,cSize*0x1000);

  FCLUST cluster_data;
  cluster_data = (FCLUST){.attrib_size = dataAttr->nresn_attributeRealSize, .cluster_size = cOffset, .cluster_offset = cSize};
  return cluster_data;
}
